#!/bin/bash

if [ -f '/usr/local/bin/bf2java' ]; then
    sudo rm '/usr/local/bin/bf2java'
fi

if [ -f '/usr/local/share/man/man1/bf2java.1' ]; then
    sudo rm '/usr/local/share/man/man1/bf2java.1'
    sudo mandb -q
fi
