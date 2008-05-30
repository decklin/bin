#!/bin/sh

root=/music

( for i in "$@"; do
    case "$i" in
        /*) path="$i";;
        .) path="$(pwd)";;
        *) path="$(pwd)/$i";;
    esac
    echo "${path##$root/}"
done ) | mpc add -

mop --showpl &
