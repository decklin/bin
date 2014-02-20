#!/bin/sh

mpath "$@" | xargs mpc update >/dev/null

if test -n "$DISPLAY" && command -v mop >/dev/null 2>&1; then
    mop --showpl &
fi
