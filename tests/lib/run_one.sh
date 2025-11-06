#!/usr/bin/env bash
set -euo pipefail
HERE="$(cd "$(dirname "$0")/.."; pwd)"
ROOT="$HERE/.."
. "$HERE/lib/colors.sh"
. "$HERE/lib/valgrind_common.sh"

slug="$1"
input="$2"

workdir="${HERE}/valgrind_logs/batch1/${slug}"
mkdir -p "$workdir"

# Prepare temp fifos/files
IN_FILE="${workdir}/input.txt"
echo -e "$input" > "$IN_FILE"

# Run in bash for expected behavior
bash_out="${workdir}/bash.out"
bash_exit="${workdir}/bash.exit"
# Use bash -lc to simulate a shell reading a line; pipe input
set +e
bash -lc "cat" < "$IN_FILE" >/dev/null 2>&1
bash -lc "$(cat "$IN_FILE")" >"$bash_out" 2>&1
echo $? > "$bash_exit"
set -e

# Run minishell under valgrind
mini_out="${workdir}/mini.out"
mini_exit="${workdir}/mini.exit"
vg_log="${workdir}/valgrind.log"

set +e
valgrind "${VALGRIND_FLAGS[@]}" --log-file="$vg_log" ./minishell < "$IN_FILE" > "$mini_out" 2>&1
ecode=$?
echo "$ecode" > "$mini_exit"
set -e

# Normalize outputs for fair diff (remove prompts/ansi)
norm_dir="${workdir}/norm"
mkdir -p "$norm_dir"
"$HERE/lib/normalize.sh" "$bash_out" > "${norm_dir}/bash.out"
"$HERE/lib/normalize.sh" "$mini_out" > "${norm_dir}/mini.out"

# Exit-status check
bash_status=$(cat "$bash_exit")
mini_status=$(cat "$mini_exit")

STATUS_OK=1
if [[ "$bash_status" != "$mini_status" ]]; then
  STATUS_OK=0
  echo -e "${RED}[EXIT]${RST} ${slug}: expected ${bash_status}, got ${mini_status}"
fi

# Output diff check
if ! diff -u "${norm_dir}/bash.out" "${norm_dir}/mini.out" > "${workdir}/diff.patch"; then
  STATUS_OK=0
  echo -e "${RED}[DIFF]${RST} ${slug}: output differs (see ${workdir}/diff.patch)"
fi

# Valgrind parsing
HEAP_OK=0
FD_OK=0
INVALID_OK=0

if grep -q "All heap blocks were freed -- no leaks are possible" "$vg_log"; then
  HEAP_OK=1
fi

if grep -q "FILE DESCRIPTORS" "$vg_log"; then
  # Extract "in use at exit: 0 descriptors" line
  if grep -E "in use at exit: +0 descriptors" "$vg_log" >/dev/null; then
    FD_OK=1
  elif grep -Eq "FILE DESCRIPTORS: +4 open \(3 std\) at exit\." "$vg_log"; then
    # Treat only-valgrind-log handle as clean
    FD_OK=1
  fi
fi

if ! grep -Eq "Invalid (read|write)|use of uninitialised value|uninitialised value" "$vg_log"; then
  INVALID_OK=1
fi

ALLOC=$(grep -E "total heap usage: " "$vg_log" | sed -E 's/.*total heap usage: ([0-9,]+) allocs, ([0-9,]+) frees.*/\1 \2/' || true)
ALLOC_N=$(echo "$ALLOC" | awk '{gsub(",",""); print $1}' 2>/dev/null)
FREES_N=$(echo "$ALLOC" | awk '{gsub(",",""); print $2}' 2>/dev/null)
ALLOC_EQ_FREES=0
if [[ -n "${ALLOC_N:-}" && -n "${FREES_N:-}" && "$ALLOC_N" == "$FREES_N" ]]; then
  ALLOC_EQ_FREES=1
fi

# Summarize CSV line
summary="${slug},${bash_status},${mini_status},${STATUS_OK},${HEAP_OK},${FD_OK},${INVALID_OK},${ALLOC_EQ_FREES}"
echo "$summary" >> "${HERE}/out/summary.csv"

# Human-friendly line
if [[ $STATUS_OK -eq 1 && $HEAP_OK -eq 1 && $FD_OK -eq 1 && $INVALID_OK -eq 1 && $ALLOC_EQ_FREES -eq 1 ]]; then
  echo -e "${GRN}[OK]${RST} ${slug}"
else
  echo -e "${RED}[FAIL]${RST} ${slug}"
fi
