#!/usr/bin/env bash
set -euo pipefail

IMG="minishell-ci"
DOCKERFILE="tools/docker/Dockerfile"

# Build image if missing
if ! docker image inspect "$IMG" >/dev/null 2>&1; then
  docker build -t "$IMG" -f "$DOCKERFILE" .
fi

# Run commands passed to this script inside the container with repo mounted at /work
# Use TTY only when available (CI may not provide one)
TTY_FLAGS=""
if [ -t 0 ] && [ -t 1 ]; then
  TTY_FLAGS="-it"
fi
docker run --rm ${TTY_FLAGS} \
  -e TERM=xterm-256color \
  -v "$PWD":/work \
  -w /work \
  "$IMG" \
  "$@"
