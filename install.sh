#!/bin/bash

if [ ! -f 'bf2java' ]; then
    ./build.sh
fi

if [ -f 'bf2java' ]; then
    if [ -d '/usr/local/bin/' ]; then
        cp 'bf2java' '/usr/local/bin/'
    fi
fi

if [ -d '/usr/local/share/man/man1/' ]; then
    cp 'bf2java.1' '/usr/local/share/man/man1/'
    mandb -q
fi
