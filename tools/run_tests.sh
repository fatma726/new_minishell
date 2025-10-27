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
run_case "export/unset" "export TEST=abc" "env | grep ^TEST=" "unset TEST" "env | grep ^TEST=" "exit"
run_case "exit status" "false" "echo $?" "exit"

echo "Redirection test"
printf "echo hi > out.txt\nexit\n" | "$BIN"
echo "cat out.txt:"; cat out.txt || true

