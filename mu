#!/bin/sh

mpwd "$@" | mpc update - >/dev/null

if [ "$DISPLAY" ] && command -v mop >/dev/null 2>&1; then
    mop --showpl &
fi
