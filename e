#!/bin/sh

# Normally, this will run Emacs in the current terminal. But if X is
# available, and we either don't have a terminal or are invoked as
# "xe", it will open up some kind of window. This will be a real X
# frame if Emacs supports X and an xterm (or, if possible, the
# Debian-configured X terminal emulator) otherwise. In all cases, if a
# server is running, we connect to it and (if supported) open a new
# frame instead of launching another instance.
#
# Why not just plain emacsclient? One, it prints an annoying message
# every time you run it without a server open. Two, it doesn't behave
# properly when given no command line arguments. We work around both
# of these by calling it once to determine if the server is up, then
# again if we really want to invoke it. (Three, I prefer requiring
# extra typing for "I want X", not for "I don't want X".)

if [ "$DISPLAY" -a \( "$(basename "$0")" = 'xe' -o ! -t 1 \) ]; then
    # This is a miserable hack. Need a nice way to check without startup.
    if ldd $(which emacs) | grep -q X; then
        newframe='-c'
    else
        newframe='-t'
        if command -v x-terminal-emulator >/dev/null 2>&1; then
            term='x-terminal-emulator -e'
        else
            term='xterm -e'
        fi
    fi
else
    args='-nw'
    newframe='-t'
fi


if emacsclient -a false -e nil >/dev/null 2>&1; then
    # If we have multihead support, use it; otherwise, see if we're in
    # a screen session set up for an older emacs server. You can try
    # to stash the current $WINDOW in a file somewhere and then return
    # to the original win from a hook, but I found that too magical.
    if emacsclient --help | grep -q create-frame; then
        clientargs="$newframe"
    elif [ "$WINDOW" ]; then
        screen -X select emacs
    fi
    if [ "$*" ]; then
        exec $term emacsclient $clientargs "$@"
    else
        exec $term emacsclient $clientargs -e nil
    fi
else
    exec $term emacs $args "$@"
fi
