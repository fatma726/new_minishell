#!/usr/bin/env bash
VALGRIND_FLAGS=(
  --trace-children=yes
  -s
  --leak-check=full
  --show-leak-kinds=all
  --track-origins=yes
  --track-fds=yes
  --error-exitcode=99
  --suppressions=valgrind_script/ignore_readline_leaks.txt
)
