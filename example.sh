#!/bin/bash

befunge="$1"
classname='Main'
jasmin="${classname}.jasmin"
class="${classname}.class"

if [ ! -f 'bf2java' ]; then
    ./build.sh
fi

if [ -f "$befunge" ]; then
    ./bf2java "$befunge" > "$jasmin"
    if [ -f "$jasmin" ]; then
        assemble "$jasmin"
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
