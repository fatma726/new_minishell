#!/bin/bash
# Manual test cases from evaluation matrix

set -e

CID=$(docker compose ps -q minishell-test)
VG_FLAGS="--leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=valgrind_script/ignore_readline_leaks.txt --error-exitcode=99"

test_case() {
    local test_name="$1"
    local input="$2"
    local expected_exit="${3:-0}"
    
    echo "=== Testing: $test_name ==="
    
    # Run with bash
    bash_out=$(echo -e "$input" | bash 2>&1)
    bash_exit=$?
    
    # Run with minishell under valgrind
    vg_log="/tmp/vg_${test_name//[^a-zA-Z0-9]/_}.log"
    mini_out=$(echo -e "$input" | docker exec -i "$CID" valgrind $VG_FLAGS --log-file="$vg_log" ./minishell 2>&1 | grep -v "^==" | grep -v "valgrind" || true)
    mini_exit=$?
    
    # Check valgrind results
    if [ -f "$vg_log" ]; then
        heap_clean=$(grep -q "All heap blocks were freed" "$vg_log" && echo "OK" || echo "LEAK")
        fd_check=$(grep -E "FILE DESCRIPTORS: [34] open" "$vg_log" >/dev/null && echo "OK" || echo "LEAK")
        errors=$(grep "ERROR SUMMARY" "$vg_log" | grep -q "0 errors" && echo "OK" || echo "ERRORS")
    else
        heap_clean="NO_LOG"
        fd_check="NO_LOG"
        errors="NO_LOG"
    fi
    
    # Compare
    echo "Bash exit: $bash_exit | Minishell exit: $mini_exit"
    echo "Bash output: [$bash_out]"
    echo "Minishell output: [$mini_out]"
    echo "Heap: $heap_clean | FD: $fd_check | Errors: $errors"
    
    if [ "$bash_exit" != "$mini_exit" ]; then
        echo "❌ EXIT CODE MISMATCH"
    fi
    
    if [ "$heap_clean" != "OK" ]; then
        echo "❌ MEMORY LEAK DETECTED"
    fi
    
    if [ "$fd_check" != "OK" ]; then
        echo "❌ FD LEAK DETECTED"
    fi
    
    echo ""
}

# Basic syntax cases
test_case "empty_line" "" 0
test_case "spaces_only" "   " 0
test_case "tab_only" "$(printf '\t')" 0
test_case "colon" ":" 0
test_case "bang" "!" 1
test_case "redir_gt_alone" ">" 2
test_case "redir_lt_alone" "<" 2
test_case "pipe_alone" "|" 2

# Echo cases
test_case "echo_basic" "echo hola" 0
test_case "echo_n_flag" "echo -n hola" 0
test_case "echo_env_var" "echo \$HOME" 0

# Redirections
test_case "echo_redirect" "echo hola > /tmp/test_redir" 0
test_case "cat_redirect" "cat < /tmp/test_redir" 0
test_case "echo_append" "echo que tal >> /tmp/test_redir" 0

# Pipes
test_case "echo_pipe_cat" "echo hola | cat" 0
test_case "echo_pipe_grep" "echo hola | grep hola" 0

# Heredoc
test_case "heredoc_basic" "cat << EOF
test
EOF" 0

# Cleanup
rm -f /tmp/test_redir

echo "=== Test Summary ==="
echo "Check individual results above"

