#!/bin/sh

if ALTERNATE_EDITOR=false emacsclient -e nil && [ "$WINDOW" ]; then
    echo "$WINDOW" > ~/var/emacs-caller
    screen -X select emacs
fi

emacsclient ${DISPLAY:+-c} "$@" || test -t 1 || xmessage 'Emacs not running'
