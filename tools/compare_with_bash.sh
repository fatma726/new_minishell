#!/usr/bin/env bash
set -euo pipefail

BIN=./minishell

if [[ ! -x "$BIN" ]]; then
  echo "Build first"; exit 1
fi

pass=0; fail=0

run_case() {
  local cmd="$1"
  local bstat mstat
  set +e
  bash -c "$cmd" >/dev/null 2>&1
  bstat=$?
  printf "%s\n" "$cmd" | "$BIN" >/dev/null 2>&1
  mstat=$?
  set -e
  if [[ "$bstat" == "$mstat" ]]; then
    echo "PASS | $cmd | bash=$bstat mini=$mstat"
    ((pass++))
  else
    echo "FAIL | $cmd | bash=$bstat mini=$mstat"
    ((fail++))
  fi
}

tests=(
  "true"
  "false"
  "echo hi"
  "pwd"
  "env"
  "cd . ; echo ok"
  "cd /nope ; echo after_cd"
  "export ABC=1 ; env | grep ^ABC="
  "unset ABC ; env | grep ^ABC="
  "export 1ABC=2"
  "unset 1ABC"
  "idontexist"
  "./idontexist"
  "/bin"
  "/bin/doesnotexist"
  "exit"
  "exit 42"
  "exit a"
  "exit 1 2"
  "echo hi > out.txt ; cat out.txt"
)

for t in "${tests[@]}"; do
  run_case "$t"
done

echo "Summary: PASS=$pass FAIL=$fail"
exit $(( fail == 0 ? 0 : 1 ))
