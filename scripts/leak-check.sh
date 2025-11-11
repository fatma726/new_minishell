#!/bin/sh
# Minimal leak check runner for 42-style evaluation (dev helper)

set -e

BIN=./minishell
SUPP1="$PWD/bin.supp"
SUPP2="$PWD/readline.supp"

if [ ! -x "$BIN" ]; then
	echo "[leak-check] minishell not built; running make..." >&2
	make -s
fi

exec valgrind \
	--leak-check=full \
	--show-leak-kinds=all \
	--leak-resolution=high \
	--track-origins=yes \
	--track-fds=yes \
	--trace-children=yes \
	--num-callers=500 \
	--suppressions="$SUPP1" \
	--suppressions="$SUPP2" \
	"$BIN"

