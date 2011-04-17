#!/bin/sh

n="$(mpc playlist | wc -l)"
ma "$@"
mpc play "$((n+1))"
