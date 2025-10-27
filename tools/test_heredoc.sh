#!/usr/bin/env bash
set -euo pipefail

THIS_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
"$THIS_DIR"/docker/run_in_container.sh bash -lc "chmod +x tools/test_heredoc_inner.sh && tools/test_heredoc_inner.sh"
