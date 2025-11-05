#!/usr/bin/env python3
import os
import pathlib
import re
import importlib.util

ROOT = pathlib.Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location("ptest", str(ROOT / "parse_all_tests.py"))
ptest = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(ptest)

tests = ptest.parse_tests_from_document()

def slugify(s: str) -> str:
    s = re.sub(r"[ /|:]", "_", s)
    s = re.sub(r"[^A-Za-z0-9_.-]", "", s)
    return s[:60]

lines = []
lines.append("#!/bin/bash")
lines.append("set -eo pipefail")
lines.append("TIMEOUT_SEC=\"${TIMEOUT_SEC:-8}\"")
lines.append("ARTIFACT_DIR=\"$PWD/tests/valgrind_logs\"")
lines.append("mkdir -p \"$ARTIFACT_DIR\"")
lines.append("ORIG=\"$PWD\"")
lines.append("tmp=$(mktemp -d)")
lines.append("cleanup() { cd \"${ORIG}\"; rm -rf \"$tmp\"; }; trap cleanup EXIT")
lines.append("cd \"$tmp\"")
lines.append("cp \"$ORIG/minishell\" .")
lines.append("chmod +x ./minishell || true")
lines.append("echo \"Amour Tu es Horrible\" > a; echo 0123456789 > b; echo Prout > c; mkdir -p srcs Docs; : > Makefile")
lines.append("VAL_BASE=\"valgrind --trace-children=yes -s --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --error-exitcode=42\"")
lines.append("if [ -f \"$ORIG/valgrind_script/ignore_readline_leaks.txt\" ]; then VAL_SUPP=\" --suppressions=$ORIG/valgrind_script/ignore_readline_leaks.txt\"; else VAL_SUPP=\"\"; fi")
lines.append("VAL_CMD=\"$VAL_BASE$VAL_SUPP\"")
lines.append("export ARTIFACT_DIR VAL_CMD")

for t in tests:
    cat = t["category"]
    desc = t["desc"]
    cmd = t["cmd"].replace("\\", "\\\\").replace("\"", "\\\"")
    bonus = t.get("bonus", False)
    inter = t.get("interactive", False)
    outdir = f"$ARTIFACT_DIR/{cat}"
    sdesc = slugify(desc)
    logfile = f"{outdir}/{sdesc}.valgrind.txt"
    lines.append(f"mkdir -p {outdir}")
    lines.append(f"# {cat} :: {desc}")
    lines.append(f"timeout \"$TIMEOUT_SEC\"s bash -lc 'printf \"%s\" \"$0\" | \"$VAL_CMD\" --log-file=\"$ARTIFACT_DIR/{cat}/{sdesc}.valgrind.txt\" ./minishell > /dev/null 2> /dev/null' \"{cmd}\" || true")

script_path = ROOT / "tools" / "valgrind_runner.sh"
pathlib.Path(script_path).write_text("\n".join(lines) + "\n")
os.chmod(script_path, 0o755)
print(f"Wrote {script_path}")
