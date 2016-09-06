#!/bin/bash
if [ ! -f out.raw ]; then
    exit
fi
rm start.oak
if [ -f start.oak ]; then
    rm start.oak
fi


sox -r 11025 -b 16 -e signed-integer -c 2 out.raw out.wav 

rm out.raw
