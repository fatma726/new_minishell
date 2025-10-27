# new_minishell

Quick usage and verification

- Build: `make re`
- Run shell: `./minishell`

Tools

- Exit-code parity: `tools/compare_with_bash.sh`
  - Compares a suite of commands against Bash and prints PASS/FAIL per case.
- Quick smoke: `tools/run_tests.sh`
  - Simple I/O checks for echo/pwd/env/export/unset and a redirection.
- Valgrind smoke: `tools/run_valgrind_smoke.sh` then `tools/verify_valgrind_clean.sh`
  - Runs a short scripted session, saves output to `/tmp/vg.out` and verifies:
    - in use at exit: 0 bytes
    - allocs == frees
    - no open FD leaks

Testing Heredoc

- Non-interactive: `tools/test_heredoc_inner.sh`
  - Covers syntax errors (`<<` and `<< WORD` without a command → status 2),
    expansion on/off based on delimiter quoting, pipelines, and multi-heredoc
    semantics (last input redirection wins).
- Interactive Ctrl-C: `tools/test_heredoc_sigint_pty.py`
  - Uses a PTY to simulate pressing Ctrl-C during heredoc.
  - Expects status 130 on interrupt and no leftover temporary heredoc files.
  - Includes a second scenario interrupting during the second heredoc of a multi-heredoc command.

Notes

- Builtins run in the parent process via a dispatcher (`src/exec/builtins_dispatch.c`) which safely
  applies stdout redirection (dup/dup2/restore) and preserves exit-code parity with Bash.
- Environment is represented as `char **env` across the execution path; export/unset are array-based.
