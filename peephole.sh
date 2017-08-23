#!/bin/bash

# see: https://stackoverflow.com/a/8574392
containsElement () {
    local e match="$1"
    shift
    for e; do [[ "$e" == "$match" ]] && return 0; done
    return 1
}

# all commonly used regexes
regexlabel='L[A-Z]+[0-9]*'
regexcomment='^;.*$'
regexlabelcolon='^'$regexlabel':$'
regexgoto='^[[:space:]]+goto[[:space:]]+'$regexlabel'$'
regexjump='^[[:space:]]+(goto|(if[_a-z]+))[[:space:]]+'$regexlabel'$'

# read file into lines
lines=()
while IFS='' read -r line || [[ -n "$line" ]]; do
    lines+=("$line")
done < <(grep -v $regexcomment "$1")

# L1:
# (and no instructions use L1)
# -->
# (empty)
function removeDeadLabels() {
    newLines=()
    usedLabels=()
    for line in "${lines[@]}"; do
        if [[ "$line" =~ $regexjump ]]; then
            label=$([[ "$line" =~ $regexlabel ]] && echo ${BASH_REMATCH[0]})
            usedLabels+=("$label")
        fi
    done
    for line in "${lines[@]}"; do
        if [[ "$line" =~ $regexlabelcolon ]]; then
            label=$([[ "$line" =~ $regexlabel ]] && echo ${BASH_REMATCH[0]})
            if containsElement "$label" "${usedLabels[@]}"; then
                newLines+=("$line")
            fi
        else
            newLines+=("$line")
        fi
    done
    lines=("${newLines[@]}")
}

# goto L1
# (code)
# L2:
# -->
# goto L1
# L2:
function removeDeadCode() {
    newLines=()
    deadCode=false
    for line in "${lines[@]}"; do
        if [[ ( "$line" =~ $regexlabelcolon ) || ( "$deadCode" == false ) ]]; then
            newLines+=("$line")
        fi
        if [[ "$line" =~ $regexlabelcolon ]]; then
            deadCode=false
        elif [[ "$line" =~ $regexgoto ]]; then
            deadCode=true
        fi
    done
    lines=("${newLines[@]}")
}

# goto/if* L1
# L1:
# -->
# L1:
function removeNullJumps() {
    newLines=()
    lastLine=""
    for line in "${lines[@]}"; do
        if [[ "$line" =~ $regexlabelcolon ]]; then
            if [[ "$lastLine" != "" ]]; then
                labelLastLine=$([[ "$lastLine" =~ $regexlabel ]] && echo ${BASH_REMATCH[0]})
                labelLine=$([[ "$line" =~ $regexlabel ]] && echo ${BASH_REMATCH[0]})
                if [[ "$labelLastLine" != "$labelLine" ]]; then
                    newLines+=("$lastLine")
                fi
            fi
            newLines+=("$line")
            lastLine=""
        else
            if [[ "$lastLine" != "" ]]; then
                newLines+=("$lastLine")
            fi
            if [[ "$line" =~ $regexjump ]]; then
                lastLine="$line"
            else
                newLines+=("$line")
                lastLine=""
            fi
        fi
    done
    lines=("${newLines[@]}")
}

# L1:
# L2:
# -->
# L1:
# (and all instructions using L2 use L1 now)
function removeRedundantLabels() {
    # TODO implement
}

# goto L1
# L2:
# goto L3
# -->
# goto L1
# (and all instructions using L2 use L3 now)
function removeRedundantIndirections() {
    # TODO implement
}

# apply all optimizations
# TODO in what order?
removeDeadLabels
removeDeadCode
removeDeadLabels
removeNullJumps
removeDeadLabels

# print lines
for line in "${lines[@]}"; do
    echo "$line"
done
