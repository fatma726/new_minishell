#!/bin/bash
# Test cases from Excel document - Mandatory only
# Uses existing run_one.sh infrastructure

set -euo pipefail

HERE="$(cd "$(dirname "$0")"; pwd)"
ROOT="$HERE/.."

CID=$(docker compose ps -q minishell-test)
if [ -z "$CID" ]; then
    echo "❌ Docker container not running. Start with: docker compose up -d"
    exit 1
fi

# Initialize summary CSV
SUMMARY_CSV="${HERE}/out/summary_excel.csv"
echo "test_name,bash_exit,mini_exit,status_ok,heap_ok,fd_ok,invalid_ok,heap_balance" > "$SUMMARY_CSV"

echo "════════════════════════════════════════════════════════════════════════"
echo "MINISHELL TEST SUITE - Excel Document Cases (Mandatory Only)"
echo "Running tests inside Docker container..."
echo "════════════════════════════════════════════════════════════════════════"

# Run tests using existing infrastructure inside Docker
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh empty_line ''" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh spaces_only '   '" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh tab_only '\t'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh colon ':'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh bang '!'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh redir_gt_alone '>'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh redir_lt_alone '<'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh redir_gtgt_alone '>>'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh redir_ltlt_alone '<<'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh pipe_alone '|'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh slash_is_dir '/'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh dot_alone '.'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh dotdot '..'" || true

# ECHO cases
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh echo_empty 'echo'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh echo_n_flag 'echo -n'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh echo_basic 'echo Hola'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh echo_n_basic 'echo -n Hola'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh echo_multiple 'echo Hola Que Tal'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh echo_var_home 'echo \$HOME'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh echo_var_question 'echo \$?'" || true

# CD cases
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh cd_home 'cd'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh cd_dot 'cd .'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh cd_dotdot 'cd ..'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh cd_root 'cd /'" || true

# PWD cases
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh pwd_basic 'pwd'" || true

# EXPORT cases
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh export_empty 'export'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh export_set 'export HOLA=bonjour'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh export_invalid 'export =bonjour'" || true

# UNSET cases
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh unset_basic 'unset HOLA'" || true

# EXIT cases
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh exit_basic 'exit'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh exit_numeric 'exit 42'" || true

# PIPE cases
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh pipe_echo_cat 'echo hola | cat'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh pipe_multiple 'echo hola | cat | cat'" || true

# REDIRECTION cases
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh redir_out 'echo test > /tmp/test_redir'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh redir_append 'echo test2 >> /tmp/test_redir'" || true
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh redir_in 'cat < /tmp/test_redir'" || true

# HEREDOC cases
docker exec "$CID" bash -c "cd /minishell && tests/lib/run_one.sh heredoc_basic 'cat << EOF
line1
line2
EOF'" || true

# Cleanup
docker exec "$CID" rm -f /tmp/test_redir 2>/dev/null || true

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "Test run complete!"
echo "Check results in: tests/out/summary.csv"
echo "Check logs in: tests/valgrind_logs/batch1/"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

# Show summary
if [ -f "${HERE}/out/summary.csv" ]; then
    echo ""
    echo "Summary:"
    tail -n +2 "${HERE}/out/summary.csv" | while IFS=',' read -r name bash_exit mini_exit status heap fd invalid balance; do
        if [ "$status" = "1" ] && [ "$heap" = "1" ] && [ "$fd" = "1" ] && [ "$invalid" = "1" ] && [ "$balance" = "1" ]; then
            echo "✅ $name"
        else
            echo "❌ $name (status=$status, heap=$heap, fd=$fd, invalid=$invalid, balance=$balance)"
        fi
    done
fi
