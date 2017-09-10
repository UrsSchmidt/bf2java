#!/bin/bash

if [ -f '/usr/local/bin/bf2java' ]; then
    rm '/usr/local/bin/bf2java'
fi

if [ -f '/usr/local/share/man/man1/bf2java.1' ]; then
    rm '/usr/local/share/man/man1/bf2java.1'
    mandb -q
fi
