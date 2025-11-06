#!/usr/bin/env bash
# usage: normalize <file>
# - remove minishell prompt lines and leading/trailing spaces
# - normalize CRLF and trailing spaces
sed -e 's/\r$//' \
    -e 's/^[[:space:]]\+$//' \
    -e 's/[[:space:]]\+$//' \
    -e '/^[[:space:]]*$/N;/^\n$/D' "$1" 2>/dev/null | sed 's/\x1b\[[0-9;]*[a-zA-Z]//g'
