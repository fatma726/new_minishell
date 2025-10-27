#!/usr/bin/env bash
set -euo pipefail

LOG=/tmp/vg.out
[[ -f "$LOG" ]] || { echo "Run tools/run_valgrind_smoke.sh first"; exit 2; }

# Extract bytes in-use (ignore commas and trailing text)
BYTES=$(grep -Eo "in use at exit: [0-9,]+ bytes" "$LOG" | tail -n1 | sed -E 's/.*: *([0-9,]+) bytes/\1/; s/,//g')
HEAP=$(grep -E "total heap usage:" "$LOG" | tail -n1)
[[ -n "$HEAP" && -n "$BYTES" ]] || { echo "No heap line found"; exit 3; }
ALLOCS=$(echo "$HEAP" | sed -E 's/.*total heap usage: *([0-9]+) allocs.*/\1/')
FREES=$( echo "$HEAP" | sed -E 's/.*total heap usage: *[0-9]+ allocs, *([0-9]+) frees.*/\1/')

if [[ "$BYTES" -ne 0 ]]; then
  echo "FAIL: in use at exit: ${BYTES} bytes"; exit 10;
fi
if [[ "$ALLOCS" != "$FREES" ]]; then
  echo "FAIL: allocs=$ALLOCS frees=$FREES"; exit 11;
fi
if grep -q "Open file descriptor" "$LOG"; then
  echo "FAIL: FD leaks detected"; grep -n "Open file descriptor" "$LOG" || true; exit 12;
fi
echo "PASS: Valgrind clean (no leaks, allocs=frees, no FD leaks)"
