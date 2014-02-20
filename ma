#!/bin/sh

mpath "$@" | mpc add

if test -n "$DISPLAY" && command -v mop >/dev/null 2>&1; then
    mop --showpl &
fi
