#!/bin/bash

# Test script to verify exit codes match bash

echo "Command,Expected Exit Code,Minishell Exit Code,Bash Exit Code,Match" > tests/out/exit_code_matrix.csv

test_exit_code() {
    local cmd="$1"
    local expected="$2"
    local desc="$3"
    
    # Test minishell (redirect stderr to /dev/null to avoid parsing issues)
    local minishell_code
    minishell_code=$(printf '%s\n' "$cmd" | ./minishell 2>/dev/null; echo $?)
    
    # Test bash (redirect stderr to /dev/null)
    local bash_code
    bash_code=$(bash -c "$cmd" 2>/dev/null; echo $?)
    
    local match="NO"
    if [ "$minishell_code" = "$bash_code" ] && [ "$minishell_code" = "$expected" ]; then
        match="YES"
    elif [ "$minishell_code" = "$bash_code" ]; then
        match="MATCH_BASH"
    fi
    
    echo "\"$desc\",$expected,$minishell_code,$bash_code,$match" >> tests/out/exit_code_matrix.csv
    echo "  $desc: minishell=$minishell_code bash=$bash_code expected=$expected [$match]"
}

mkdir -p tests/out

echo "Testing exit codes..."

# Basic commands
test_exit_code "true" 0 "true"
test_exit_code "false" 1 "false"

# Exit command variations
test_exit_code "exit" 0 "exit (no args)"
test_exit_code "exit 42" 42 "exit 42"
test_exit_code "exit -1" 255 "exit -1"
test_exit_code "exit 999999" 63 "exit 999999 (overflow)"
test_exit_code "exit abc" 2 "exit abc (invalid)"

# Command errors
test_exit_code "ls /nonexisting/dir" 2 "ls nonexisting"
test_exit_code "/" 126 "slash is directory"
test_exit_code "unknowncmd123" 127 "command not found"

# Signals (these need manual testing)
echo "\"cat <<EOF + Ctrl-C\",130,N/A,N/A,MANUAL" >> tests/out/exit_code_matrix.csv
echo "\"Ctrl-\\\\ on process\",131,N/A,N/A,MANUAL" >> tests/out/exit_code_matrix.csv

echo ""
echo "Results saved to tests/out/exit_code_matrix.csv"
cat tests/out/exit_code_matrix.csv

