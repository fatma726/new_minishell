#!/usr/bin/env bash
set -euo pipefail

# Minimal, deterministic heredoc tests (non-interactive).
# Each test feeds scripted input to minishell, appends 'echo __RC:$?' inside
# to capture the shell's last status, then checks output and status.
# Exits non-zero if any test fails.

MS=./minishell
LOG_DIR=./test-logs
LOG_FILE="${LOG_DIR}/heredoc.log"
mkdir -p "$LOG_DIR"
: > "$LOG_FILE"

have() { command -v "$1" >/dev/null 2>&1; }

fail() { echo ":: FAIL: $*"; echo ":: FAIL: $*" >> "$LOG_FILE"; exit 1; }

# run one case:
# t "<name>" <expected_status> "<grep_pattern_or_empty>"  [ -- <line1> <line2> ... ]
t() {
  local name="$1"; shift
  local exp_rc="$1"; shift
  local pattern="$1"; shift
  if [[ "$1" != "--" ]]; then
    echo "internal: missing '--' before lines for case '$name'" >&2
    exit 2
  fi
  shift

  # compose test script: provided lines, then echo __RC:$?, then exit
  local tmp; tmp="$(mktemp /tmp/ms_case.XXXXXX.txt)"
  : > "$tmp"
  while (( "$#" )); do printf "%s\n" "$1" >> "$tmp"; shift; done
  printf "echo __RC:\$?\n" >> "$tmp"
  printf "exit\n" >> "$tmp"

  # run minishell (non-TTY) and capture output
  local out; out="$("$MS" < "$tmp" 2>&1 || true)"

  # extract last status printed inside minishell
  local rc
  rc="$(grep -oE '__RC:[0-9]+' <<<"$out" | tail -1 | cut -d: -f2 || true)"
  [[ -z "$rc" ]] && rc="(missing)"

  # log raw for debugging
  {
    echo "==== $name ===="
    echo "$out"
    echo "---- parsed_rc=$rc (expected $exp_rc)"
  } >> "$LOG_FILE"

  # checks
  local ok=1
  if [[ "$rc" != "$exp_rc" ]]; then
    echo "[FAIL] $name: status expected=$exp_rc got=$rc"
    echo "$out" | sed -n '1,120p'
    ok=0
  fi
  if [[ -n "$pattern" ]]; then
    if ! grep -Eq "$pattern" <<<"$out"; then
      echo "[FAIL] $name: pattern not found: $pattern"
      echo "$out" | sed -n '1,120p'
      ok=0
    fi
  fi

  rm -f "$tmp"
  if [[ $ok -eq 1 ]]; then
    echo "[PASS] $name"
    echo "[PASS] $name" >> "$LOG_FILE"
    return 0
  else
    echo "[FAIL] $name" >> "$LOG_FILE"
    exit 10
  fi
}

[[ -x "$MS" ]] || fail "minishell binary not found"

##############################################################################
#                            TEST CASES (Mandatory)
##############################################################################

# Syntax errors (return 2); message contains backticks around token
t "syntax: bare << (newline)" 2 "syntax error near unexpected token \`newline'" -- \
  "<<"

t "syntax: stray heredoc without command" 2 "syntax error near unexpected token \`<<'" -- \
  "<< EOF"

# Normal heredoc
t "heredoc: cat prints lines" 0 "^line1$|^line2$" -- \
  "cat << EOF" \
  "line1" \
  "line2" \
  "EOF"

# Expansion ON for unquoted delimiter
t "heredoc: expands with unquoted delim" 0 "^ok$" -- \
  "export FOO=ok" \
  "cat << EOF" \
  "\$FOO" \
  "EOF"

# No expansion when delimiter is single-quoted
t 'heredoc: no expand with single-quoted delim' 0 '^\$FOO$' -- \
  "export FOO=ok" \
  "cat << 'EOF'" \
  "\$FOO" \
  "EOF"

# Pipeline + heredoc on rhs
t "pipe+heredoc: rhs provides data" 0 "^a$" -- \
  "echo hi | cat <<X" \
  "a" \
  "X"

# Multiple heredocs: last input redirection wins (stdin comes from last <<)
t "multi-heredoc: last one wins (prints B only)" 0 "^X to B$" -- \
  "cat <<A <<B" \
  "X to B" \
  "B" \
  "Y to A" \
  "A"

t "multi-heredoc: last one wins (prints A only)" 0 "^Y to A$" -- \
  "cat <<B <<A" \
  "X to B" \
  "B" \
  "Y to A" \
  "A"

# Directory execute -> 126
t "exec: /bin is directory -> 126" 126 "" -- \
  "/bin"

# Not found -> 127
t "exec: missing absolute path -> 127" 127 "" -- \
  "/bin/doesnotexist"

echo "---- LOG saved to $LOG_FILE"
