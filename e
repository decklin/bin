#!/bin/sh

# Normally, this will run Emacs in the current terminal. But if X is
# available, and we either don't have a terminal or are invoked as
# "xe" (instead of the usual "e" = $EDITOR alias), it will open up
# some kind of window. This will be a real X frame if Emacs supports X
# and an xterm (or, if possible, the Debian-configured X terminal
# emulator) otherwise.
#
# In all cases, if a server is running, we connect to it and open a
# new frame (tty or X) instead of launching a new Emacs. Emacs 23 is
# required for opening a new "head" on the server process; if you
# don't have it, get rid of XT and EC_ARGS and do whatever you were
# doing to switch over to your server manually.
#
# Why not just plain emacsclient? One, it prints an annoying message
# every time you run it without a server open. Two, it doesn't behave
# properly when given no command line arguments. We work around both
# of these by calling it once to determine if the server is up, then
# again if we really want to invoke it.

if [ "$DISPLAY" -a \( "$(basename "$0")" = 'xe' -o ! -t 0 \) ]; then
    # FIXME: this loses. so does checking window-system in --batch --eval
    # since there is no frame yet.
    if ldd $(which emacs) | grep -q X11; then
        EC_ARGS='-c'
    else
        EC_ARGS='-t'
        if command -v x-terminal-emulator >/dev/null 2>&1; then
            XT='x-terminal-emulator -e'
        else
            XT='xterm -e'
        fi
    fi
else
    E_ARGS='-nw'
    EC_ARGS='-t'
fi

if emacsclient -a false --eval nil >/dev/null 2>&1; then
    if [ "$*" ]; then
        exec $XT emacsclient $EC_ARGS "$@"
    else
        exec $XT emacsclient $EC_ARGS --eval nil
    fi
else
    if [ "$(basename "$0")" = 'xe' -a -t 0 ]; then
        $XT emacs $E_ARGS "$@" &
    else
        exec $XT emacs $E_ARGS "$@"
    fi
fi
