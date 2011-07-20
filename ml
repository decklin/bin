#!/bin/sh

len="$(mpc playlist | wc -l | wc -c)"
fmt="${*:-[%name%|[%artist% - ]%title%|%file%] (%time%)}"
cur="$(mpc current --format="$fmt")"

mpc playlist --format="%position% $fmt" | while read pos song; do
    if [ -t 1 -a "$song" = "$cur" ]; then
        color_on='\033[1;36m'
        color_off='\033[0m'
    else
        color_on=''
        color_off=''
    fi
    printf ${color_on}"%${len}d  %s${color_off}\n" "$pos" "$song"
done
