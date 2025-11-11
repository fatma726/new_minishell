#!/bin/sh
# Functional checker: run 42-like cases against minishell and bash

set -e

MINISHELL=./minishell
CASES_FILE=scripts/cases_42_scale.txt
OUTDIR=functional_logs

mkdir -p "$OUTDIR"

sanitize() {
  # Replace any char not in A-Za-z0-9_.- with underscore
  # Use POSIX sed trick
  printf '%s' "$1" | LC_ALL=C sed 's/[^A-Za-z0-9_.-]/_/g'
}

normalize_minishell() {
  # Remove CR, drop prompt lines and empty lines
  sed 's/\r$//' | \
  grep -v -E '^[[:space:]]*(minishell>|minishell)' | \
  grep -v -E '^[[:space:]]*$'
}

normalize_bash() {
  # Remove CR and empty lines
  sed 's/\r$//' | \
  grep -v -E '^[[:space:]]*$'
}

run_one_case() {
  num="$1"
  name="$2"
  casefile="$3"

  safe=$(sanitize "$name")

  ms_out=$(mktemp)
  ms_err=$(mktemp)
  ms_out_n=$(mktemp)
  ms_err_n=$(mktemp)

  b_out=$(mktemp)
  b_err=$(mktemp)
  b_out_n=$(mktemp)
  b_err_n=$(mktemp)

  # Run minishell
  (cat "$casefile") | $MINISHELL >"$ms_out" 2>"$ms_err" || true
  ms_status=$?

  # Run bash
  bash < "$casefile" >"$b_out" 2>"$b_err" || true
  b_status=$?

  # Normalize
  normalize_minishell < "$ms_out" > "$ms_out_n"
  normalize_minishell < "$ms_err" > "$ms_err_n"
  normalize_bash < "$b_out" > "$b_out_n"
  normalize_bash < "$b_err" > "$b_err_n"

  ok=1

  # Diff stdout
  stdout_diff="$OUTDIR/${num}-${safe}.stdout.diff"
  if ! diff -u "$b_out_n" "$ms_out_n" > "$stdout_diff" 2>&1; then
    ok=0
  else
    rm -f "$stdout_diff"
  fi

  # Diff stderr
  stderr_diff="$OUTDIR/${num}-${safe}.stderr.diff"
  if ! diff -u "$b_err_n" "$ms_err_n" > "$stderr_diff" 2>&1; then
    ok=0
  else
    rm -f "$stderr_diff"
  fi

  # Diff status
  if [ "$b_status" -ne "$ms_status" ]; then
    ok=0
    printf 'bash=%s minishell=%s\n' "$b_status" "$ms_status" > "$OUTDIR/${num}-${safe}.status.diff"
  else
    rm -f "$OUTDIR/${num}-${safe}.status.diff" 2>/dev/null || true
  fi

  if [ "$ok" -eq 1 ]; then
    printf '[OK] %s %s\n' "$num" "$name"
  else
    printf '[FAIL] %s %s\n' "$num" "$name"
  fi

  # Cleanup temps
  rm -f "$ms_out" "$ms_err" "$ms_out_n" "$ms_err_n" \
        "$b_out" "$b_err" "$b_out_n" "$b_err_n"
}

# Parse cases file
if [ ! -f "$CASES_FILE" ]; then
  echo "[ERROR] Cases file not found: $CASES_FILE" >&2
  exit 1
fi

count=0
in_case=0
name=""
tmpcase=""

while IFS= read -r line || [ -n "$line" ]; do
  case "$line" in
    @case\ *)
      in_case=1
      name=${line#@case }
      tmpcase=$(mktemp)
      : > "$tmpcase"
      ;;
    @end)
      if [ "$in_case" -eq 1 ]; then
        count=$((count+1))
        run_one_case "$count" "$name" "$tmpcase"
        rm -f "$tmpcase"
        in_case=0
        name=""
        tmpcase=""
      fi
      ;;
    *)
      if [ "$in_case" -eq 1 ]; then
        # Preserve line exactly
        printf '%s\n' "$line" >> "$tmpcase"
      fi
      ;;
  esac
done < "$CASES_FILE"

echo "All functional cases done. Check $OUTDIR/ for diffs."

