#!/usr/bin/env bash
set -euo pipefail

LOG=/tmp/vg.out
[[ -f "$LOG" ]] || { echo "Run tools/run_valgrind_smoke.sh first"; exit 2; }

IN_USE=$(grep -Eo "in use at exit: [^,]+" "$LOG" | tail -n1)
HEAP=$(grep -E "total heap usage:" "$LOG" | tail -n1)
[[ -n "$HEAP" ]] || { echo "No heap line found"; exit 3; }
ALLOCS=$(echo "$HEAP" | sed -E 's/.*total heap usage: *([0-9]+) allocs.*/\1/')
FREES=$( echo "$HEAP" | sed -E 's/.*total heap usage: *[0-9]+ allocs, *([0-9]+) frees.*/\1/')

if [[ "$IN_USE" != "in use at exit: 0 bytes" ]]; then
  echo "FAIL: $IN_USE"; exit 10;
fi
if [[ "$ALLOCS" != "$FREES" ]]; then
  echo "FAIL: allocs=$ALLOCS frees=$FREES"; exit 11;
fi
if grep -q "Open file descriptor" "$LOG"; then
  echo "FAIL: FD leaks detected"; grep -n "Open file descriptor" "$LOG" || true; exit 12;
fi
echo "PASS: Valgrind clean (no leaks, allocs=frees, no FD leaks)"

