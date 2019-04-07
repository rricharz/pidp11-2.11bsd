#!/bin/sh
# rricharz 2019

for filename in *.plt
do
    # erase screen
    printf '\033\014'
    echo tek4010 is displaying "$filename"
    cat "$filename"
    # let tek4010 wait 5 seconds (not this script, which just shuffles stuff into the buffer!)
    for i in 1 2 3 4 5 6 7 8 9 10
    do
        for j in 1 2 3 4 5 6 7 8 9 10
        do
            printf '\007'
        done
    done
done



