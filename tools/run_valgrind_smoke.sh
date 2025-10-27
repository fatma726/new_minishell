#!/usr/bin/env bash
set -euo pipefail

SCRIPT='/tmp/ms_script.txt'
cat > "$SCRIPT" <<'EOS'
echo hi
export X=1
unset X
pwd
exit
EOS

valgrind --trace-children=yes -s \
  --run-libc-freeres=yes \
  --leak-check=full --show-leak-kinds=all \
  --track-origins=yes --track-fds=yes \
  --suppressions=valgrind_script/ignore_readline_leaks.txt \
  ./minishell < "$SCRIPT" 2>&1 | tee /tmp/vg.out

echo "----- HEAP SUMMARY -----"
sed -n '/HEAP SUMMARY:/,$p' /tmp/vg.out
