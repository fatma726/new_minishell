#!/usr/bin/env bash
set -euo pipefail

# Run from repo root regardless of invocation path
cd "$(dirname "$0")/.."

printf "cat << ok\n\003\n" | ./minishell >/dev/null 2>&1 || rc=$?
rc=${rc:-0}
if [ "$rc" -ne 130 ]; then
  echo "FAIL: expected 130, got $rc"
  exit 1
fi
echo "PASS: non-tty heredoc Ctrl-C -> 130"

