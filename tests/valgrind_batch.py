#!/usr/bin/env python3
"""Automated Valgrind batch for minishell mandatory behaviors."""
from __future__ import annotations

import re
import shutil
import subprocess
from datetime import datetime
from pathlib import Path

TEST_CASES = [
    (
        "pwd_basic",
        "pwd\n"
        "echo $?\n"
        "exit\n",
        0,
    ),
    (
        "echo_options",
        "echo -n HelloWorld\n"
        "echo $?\n"
        "exit\n",
        0,
    ),
    (
        "env_export_unset",
        "export FOO=bar\n"
        "echo $FOO\n"
        "unset FOO\n"
        "echo $?\n"
        "exit\n",
        0,
    ),
    (
        "cd_chain",
        "pwd\n"
        "cd ..\n"
        "pwd\n"
        "cd -\n"
        "pwd\n"
        "echo $?\n"
        "exit\n",
        0,
    ),
    (
        "pipe_simple",
        "echo pipeline | tr a-z A-Z\n"
        "echo $?\n"
        "exit\n",
        0,
    ),
    (
        "redir_out_append",
        "echo first > /tmp/ms_valgrind_out\n"
        "cat /tmp/ms_valgrind_out\n"
        "echo second >> /tmp/ms_valgrind_out\n"
        "cat /tmp/ms_valgrind_out\n"
        "rm /tmp/ms_valgrind_out\n"
        "echo $?\n"
        "exit\n",
        0,
    ),
    (
        "redir_in",
        "cat < Makefile | head -n 1\n"
        "echo $?\n"
        "exit\n",
        0,
    ),
    (
        "heredoc_basic",
        "cat << EOF\n"
        "line1\n"
        "EOF\n"
        "echo $?\n"
        "exit\n",
        0,
    ),
    (
        "builtin_exit_status",
        "false\n"
        "echo $?\n"
        "true\n"
        "echo $?\n"
        "exit\n",
        0,
    ),
    (
        "exit_numeric",
        "exit 42\n",
        42,
    ),
]

VALGRIND_FLAGS = [
    "--trace-children=yes",
    "-s",
    "--leak-check=full",
    "--show-leak-kinds=all",
    "--track-origins=yes",
    "--track-fds=yes",
    "--suppressions=valgrind_script/ignore_readline_leaks.txt",
]

LOG_DIR = Path("tests/valgrind_logs/batch1")
SUMMARY_PATH = Path("tests/out/valgrind_summary.txt")


def run_case(slug: str, script: str, expected_exit: int) -> dict:
    cmd = ["valgrind", *VALGRIND_FLAGS, "./minishell"]
    result = subprocess.run(
        cmd,
        input=script.encode("utf-8"),
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        check=False,
    )
    output = result.stdout.decode("utf-8", errors="replace")
    (LOG_DIR / f"{slug}.valgrind.txt").write_text(output)

    lines = output.splitlines()
    summary = {
        "slug": slug,
        "returncode": result.returncode,
        "expected": expected_exit,
        "output": output,
        "checks": {},
        "heap_line": "",
        "fd_line": "",
        "error_line": "",
    }

    summary["checks"]["heap_free"] = (
        "All heap blocks were freed -- no leaks are possible" in output
    )
    summary["checks"]["invalid_rw"] = (
        "Invalid read" not in output and "Invalid write" not in output
    )

    def find_line(keyword: str) -> str:
        return next((line for line in lines if keyword in line), "")

    summary["fd_line"] = find_line("FILE DESCRIPTORS")
    summary["checks"]["fds"] = (
        "0 open at exit" in summary["fd_line"] if summary["fd_line"] else False
    )

    summary["heap_line"] = find_line("total heap usage")
    allocs = frees = None
    if summary["heap_line"]:
        match = re.search(
            r"total heap usage:\\s*([\\d,]+)\\s*allocs,\\s*([\\d,]+)\\s*frees",
            summary["heap_line"],
        )
        if match:
            allocs = int(match.group(1).replace(",", ""))
            frees = int(match.group(2).replace(",", ""))
    summary["checks"]["heap_balance"] = allocs == frees if allocs is not None else False

    summary["error_line"] = find_line("ERROR SUMMARY")
    summary["checks"]["errors"] = (
        "0 errors" in summary["error_line"] if summary["error_line"] else False
    )
    summary["checks"]["exit_code"] = result.returncode == expected_exit

    return summary


def main() -> None:
    if LOG_DIR.exists():
        shutil.rmtree(LOG_DIR)
    LOG_DIR.mkdir(parents=True, exist_ok=True)
    SUMMARY_PATH.parent.mkdir(parents=True, exist_ok=True)

    subprocess.run(["rm", "-f", "/tmp/ms_valgrind_out"], check=False)

    summaries = []
    pass_count = 0

    for slug, script, expected_exit in TEST_CASES:
        print(f"[VALGRIND] Running {slug}...")
        summary = run_case(slug, script, expected_exit)
        summaries.append(summary)
        if all(summary["checks"].values()):
            pass_count += 1
        else:
            print(f"[VALGRIND] {slug} FAILED: {summary['checks']}")

    total = len(TEST_CASES)
    with SUMMARY_PATH.open("w", encoding="utf-8") as fh:
        now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        fh.write(f"Valgrind batch summary - {now}\n")
        fh.write("Flags: " + " ".join(VALGRIND_FLAGS) + " ./minishell\n")
        fh.write(f"Total tests: {total}\n")
        fh.write(f"Pass: {pass_count}\n")
        fh.write(f"Fail: {total - pass_count}\n\n")
        for summary in summaries:
            checks = ", ".join(
                f"{name}={'ok' if ok else 'FAIL'}" for name, ok in summary["checks"].items()
            )
            fh.write(
                f"{summary['slug']}: {'PASS' if all(summary['checks'].values()) else 'FAIL'}"
                f" (exit {summary['returncode']} expected {summary['expected']}; {checks})\n"
            )
            if summary["heap_line"]:
                fh.write(f"  {summary['heap_line']}\n")
            if summary["fd_line"]:
                fh.write(f"  {summary['fd_line']}\n")
            if summary["error_line"]:
                fh.write(f"  {summary['error_line']}\n")
            fh.write("\n")

    print(f"[VALGRIND] Completed: {pass_count}/{total} passing.")


if __name__ == "__main__":
    main()
