#!/bin/sh

mpc update "$(mpwd "$@")" >/dev/null

if [ "$DISPLAY" ] && command -v mop >/dev/null 2>&1; then
    mop --showpl &
fi
