#!/bin/bash

CID=$(docker compose ps -q minishell-test)
VG_FLAGS="--leak-check=full --show-leak-kinds=all --track-fds=yes --suppressions=valgrind_script/ignore_readline_leaks.txt"

test_case() {
    local name="$1"
    local input="$2"
    
    echo "=========================================="
    echo "Testing: $name"
    echo "Input: [$input]"
    echo "------------------------------------------"
    
    # Run bash
    bash_out=$(printf "%s\n" "$input" | bash 2>&1)
    bash_exit=$?
    
    # Run minishell with valgrind
    vg_log="/tmp/vg_${name}.log"
    mini_out=$(printf "%s\n" "$input" | docker exec -i "$CID" bash -c "valgrind $VG_FLAGS --log-file=$vg_log ./minishell 2>&1" | grep -v "^==" | grep -v "valgrind" || true)
    mini_exit=$?
    
    # Check valgrind results
    echo "Valgrind summary:"
    docker exec -t "$CID" bash -c "if [ -f $vg_log ]; then tail -10 $vg_log | grep -E '(heap|FILE DESCRIPTORS|ERROR SUMMARY|All heap)' || true; fi"
    
    echo ""
    echo "Bash:   exit=$bash_exit | output=[$bash_out]"
    echo "Mini:   exit=$mini_exit | output=[$mini_out]"
    
    if [ "$bash_exit" != "$mini_exit" ]; then
        echo "⚠️  EXIT CODE MISMATCH"
    else
        echo "✅ Exit codes match"
    fi
    
    echo ""
}

# Basic cases
test_case "empty_line" ""
test_case "spaces" "   "
test_case "colon" ":"
test_case "bang" "!"
test_case "redir_gt" ">"
test_case "redir_lt" "<"
test_case "pipe_alone" "|"

# Echo cases  
test_case "echo_basic" "echo hola"
test_case "echo_n" "echo -n hola"
test_case "echo_var" "echo \$HOME"

# Redirections
test_case "echo_redirect" "echo test > /tmp/t1"
test_case "cat_redirect" "cat < /tmp/t1"

# Pipes
test_case "echo_pipe" "echo hola | cat"
test_case "echo_pipe_grep" "echo hola | grep hola"

# Cleanup
docker exec -t "$CID" rm -f /tmp/t1

echo "=========================================="
echo "Test run complete"

