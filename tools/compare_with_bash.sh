#!/usr/bin/env bash
set -euo pipefail

BIN=./minishell

[[ -x "$BIN" ]] || { echo "Build first"; exit 1; }

FAIL=0

bash_status() {
  /usr/bin/env bash -lc "$1" >/dev/null 2>&1; printf "%d" "$?"
}

run_case () {
  local CMD="$1"; local bs ms
  bs="$(bash_status "$CMD")"
  set +e
  printf "%s\n" "$CMD" | "$BIN" >/dev/null 2>&1; ms=$?
  set -e
  if [[ "$bs" == "$ms" ]]; then
    printf "[PASS] %-45s bash=%s mini=%s\n" "$CMD" "$bs" "$ms"
  else
    printf "[FAIL] %-45s bash=%s mini=%s\n" "$CMD" "$bs" "$ms"
    FAIL=1
  fi
}

run_case "true"
run_case "false"
run_case "idontexist"
run_case "/bin"
run_case "/bin/doesnotexist"
run_case "echo hi"
run_case "pwd"
run_case "env"
# minishell may not support ';' sequencing; test cd outcomes directly
run_case "cd ."
run_case "cd /no-such-dir"
run_case "export 1ABC=2"
run_case "unset 1ABC"
run_case "exit"
run_case "exit 42"
run_case "exit a"
run_case "exit 1 2; echo x"
run_case "echo hi > out.txt; cat out.txt"
run_case "| ls"
run_case ">> |"
run_case "echo \"unterminated"

exit $FAIL
