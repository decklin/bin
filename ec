#!/bin/sh

pidof emacs >/dev/null || emacs --daemon 2>/dev/null
emacsclient ${DISPLAY:+-c} "$@"
