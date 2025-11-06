#!/usr/bin/env bash
set -euo pipefail
HERE="$(cd "$(dirname "$0")"; pwd)"
. "$HERE/lib/colors.sh"

manifest="${1:-$HERE/manifest.csv}"
if [[ ! -f "$manifest" ]]; then
  echo -e "${RED}Manifest not found:${RST} $manifest"
  exit 2
fi

# reset summary
echo "slug,bash_exit,mini_exit,status_ok,heap_clean,fd_clean,invalid_free,alloc_eq_frees" > "$HERE/out/summary.csv"

# read CSV (skip header)
tail -n +2 "$manifest" | while IFS=',' read -r slug input; do
  slug_trim=$(echo "$slug" | sed 's/[[:space:]]//g')
  if [[ -z "$slug_trim" ]]; then
    continue
  fi
  # Unescape \n tokens to real newlines for multi-line entries (heredocs etc.)
  input_eval=$(printf "%b" "$input")
  "$HERE/lib/run_one.sh" "$slug_trim" "$input_eval"
done

# Build human summary table
python3 - "$HERE/out/summary.csv" << 'PY'
import sys, pandas as pd
df = pd.read_csv(sys.argv[1])
ok = (df[['status_ok','heap_clean','fd_clean','invalid_free','alloc_eq_frees']]==1).all(axis=1).sum()
total = len(df)
rate = 0 if total==0 else round(100*ok/total,1)
print(f"\n==== SUMMARY ====\nPASS: {ok}/{total} ({rate}%)")
print(df.to_string(index=False))
PY
