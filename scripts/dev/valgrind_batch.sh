#!/usr/bin/env bash
set -euo pipefail

VAL="valgrind --leak-check=full --leak-resolution=high -s --track-origins=yes \
  --num-callers=500 --show-mismatched-frees=yes --show-leak-kinds=all \
  --track-fds=yes --trace-children=yes --gen-suppressions=no --error-limit=no \
  --undef-value-errors=yes --expensive-definedness-checks=yes \
  --read-var-info=yes --keep-debuginfo=yes \
  --suppressions=\"$PWD/bin.supp\" --suppressions=\"$PWD/readline.supp\" \
  ./minishell"

LOGDIR="valgrind_logs"
mkdir -p "$LOGDIR"

if ! command -v valgrind >/dev/null 2>&1; then
  echo "[ERROR] valgrind not found. Please run this batch inside the Docker image with valgrind installed." >&2
  exit 127
fi

sanitize() {
  # Replace any char not in A-Za-z0-9_.- with underscore
  echo "$1" | LC_ALL=C sed 's/[^A-Za-z0-9_.-]/_/g'
}

run_case() {
  local label=$1; shift
  local cmd=$1
  local safe; safe=$(sanitize "$label")
  echo "==== CASE: $label ===="
  printf "%s\nexit\n" "$cmd" | eval "$VAL" 2>&1 | tee "$LOGDIR/${safe}.log"
}

run_case "/bin/ls" "/bin/ls"
run_case "empty" ""
run_case "spaces" "    "
run_case "tabs" "\t\t\t"
run_case "/bin/ls -la" "/bin/ls -la"
run_case "echo dq" "echo \"hello world\""
run_case "echo sq" "echo '\''hello world'\''"
run_case "echo" "echo"
run_case "echo (second)" "echo"
run_case "echo hi" "echo hi"
run_case "echo -n hi" "echo -n hi"
run_case "exit" "exit"
run_case "exit again" "exit"
run_case "exit 42" "exit 42"
run_case "ls noexist ; $?" "/bin/ls /doesnotexist; echo $?"
run_case "false ; $?" "false; echo $?"
run_case "true ; $?" "true; echo $?"
run_case "ls ; ls noexist ; $?" "/bin/ls; /bin/ls /doesnotexist; echo $?"
run_case "surprise1" "echo '\''"'"'$USER'"'"'"'"
run_case "surprise2" "echo '"\"\$USER\"'"
run_case "env" "env"
run_case "export TEST=42" "export TEST=42"
run_case "export TEST=43" "export TEST=43"
run_case "unset TEST" "unset TEST"
run_case "cd/ pwd" "cd / ; pwd"
run_case "cd fail" "cd /doesnotexist"
run_case "unset PATH ; ls" "unset PATH; ls"
run_case "echo PATH status" "unset PATH; ls; echo $?"
run_case "> redir" "echo hi > out.txt"
run_case "< redir" "cat < out.txt"
run_case ">> redir" "echo hi >> out.txt"
run_case "heredoc" $'cat << EOF\nhello\nEOF'
run_case "heredoc pipe" $'cat << EOF | cat\nhello\nEOF'
run_case "pipe" "echo hi | cat"
run_case "pipe 2" "echo hi | cat | cat"
run_case "pipe fail left" "/bin/ls /nope | cat"

echo "==== DONE ===="
run_case "dq complex" "echo \"cat lol.c | cat > lol.c\""
run_case "dq HOME" "echo \"$HOME\""
run_case "sq USER" "echo '$USER'"
run_case "sq pipe word" "echo 'hello | world'"
run_case "random wrong" "dsbksdg"
run_case "very long line" "$(python3 - <<'PY'
print('x'*5000)
PY
)"
