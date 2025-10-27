#!/usr/bin/env bash
set -euo pipefail

ROOT="${1:-.}"
LIST="tools/forbidden_functions.txt"

if [[ ! -f "$LIST" ]]; then
  echo "Missing $LIST"; exit 2
fi

FAIL=0
while IFS= read -r sym; do
  [[ -z "$sym" || "$sym" =~ ^# ]] && continue
  if grep -R --line-number --include="*.[ch]" -E "[^a-zA-Z0-9_]${sym}[[:space:]]*\\(" "$ROOT" >/tmp/forbid_hits 2>/dev/null; then
    echo "Forbidden usage detected: $sym"
    cat /tmp/forbid_hits
    echo "---"
    FAIL=1
  fi
done < "$LIST"

if [[ "$FAIL" -ne 0 ]]; then
  echo "FAIL: forbidden APIs found"; exit 1
else
  echo "PASS: no forbidden APIs found"
fi

