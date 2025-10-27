#!/usr/bin/env bash
set -euo pipefail

BIN=./minishell
SUP=valgrind_script/ignore_readline_leaks.txt

if [[ ! -x "$BIN" ]]; then
  echo "Build first"; exit 1
fi

printf "echo hi\npwd\nenv\nexit\n" |
valgrind --trace-children=yes -s \
  --suppressions="$SUP" \
  --leak-check=full --show-leak-kinds=all \
  --track-origins=yes --track-fds=yes "$BIN" 2>&1 | sed -n '/HEAP SUMMARY:/,/$/p'

