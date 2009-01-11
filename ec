#!/bin/sh

if ALTERNATE_EDITOR=false emacsclient -e nil; then
    emacsrunning=y
fi

if [ "$emacsrunning" -a "$WINDOW" ]; then
    echo "$WINDOW" > ~/var/emacs-caller
    screen -X select emacs
fi

emacsclient ${DISPLAY:+-c} "$@"
