#!/bin/sh

mpc playlist --format '%file%' | sed 's/^/\/srv\/music\//;s/.*/"&"/'
