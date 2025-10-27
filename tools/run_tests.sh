#!/usr/bin/env bash
set -euo pipefail

BIN=./minishell

if [[ ! -x "$BIN" ]]; then
  echo "Build first"; exit 1
fi

run_case() {
  local name="$1"; shift
  local input="$*"
  echo "--- $name ---"
  printf "%s\n" "$input" | "$BIN"
}

run_case "echo hi" "echo hi" "exit"
run_case "pwd" "pwd" "exit"
run_case "env" "env" "exit"
run_case "export" "export TEST=abc" "env" "exit"
run_case "unset" "export TEST=abc" "unset TEST" "env" "exit"
run_case "exit status" "false" "echo $?" "exit"

echo "Redirection test"
printf "echo hi > out.txt\nexit\n" | "$BIN"
echo "out.txt content:"; cat out.txt || true
