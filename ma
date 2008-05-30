#!/bin/sh

mpwd "$@" | mpc add -

if [ "$DISPLAY" ] && command -v mop >/dev/null 2>&1; then
    mop --showpl &
fi
