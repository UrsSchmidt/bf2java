#!/bin/bash

befunge='examples/esolang/deadfish.txt'
classname='Main'
jasmin="${classname}.j"
class="${classname}.class"
assemble='/home/urs/github-clones/Krakatau/assemble.py'

if [ ! -f 'bf2java' ]; then
    ./build.sh
fi

if [ -f "$befunge" ]; then
    ./bf2java "$befunge" > "$jasmin"
    if [ -f "$jasmin" ]; then
        python "$assemble" -q "$jasmin" >/dev/null
        if [ -f "$class" ]; then
            java -noverify "$classname"
        else
            echo "File not found $class"
        fi
    else
        echo "File not found $jasmin"
    fi
else
    echo "File not found $befunge"
fi

if [ -f "$class" ]; then
    rm "$class"
fi

if [ -f "$jasmin" ]; then
    rm "$jasmin"
fi
