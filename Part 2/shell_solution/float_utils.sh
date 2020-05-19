#! /bin/sh

# Some utility functions to compare / manipulate flosingpoint numbers 
# without requiring `expr` or other sub-shells.

split_int () {
    # split a floating point number $1 to integers and millionths
    # use the global variables intpart and fracpart to return the split values
    # to save `` and a sub-process
    # wish there was equivalent of index() in BASH
    case "$1" in 
    .*) intpart=0; fracpart=${1:1};;
    ?.*) intpart=${1:0:1}; fracpart=${1:2};;
    ??.*) intpart=${1:0:2}; fracpart=${1:3};;
    ???.*) intpart=${1:0:3}; fracpart=${1:4};;
    *.*) echo "Cant parse real $1" 1>&2; exit 1;;
    [0-9]) intpart=$1; fracpart=0;;
    [0-9]*[0-9]) intpart=$1; fracpart=0;;
    *) echo "Cant parse real $1" 1>&2; exit 1;;
    esac 

    [ ${#fracpart} -gt 6 ] && {
        echo "Frac part of $1 too long" 1>&2
        exit 1
    }

    # need to convert to millionths then strip the leading 0
    fracpart=${fracpart}000000
    fracpart=${fracpart:0:6};
    
    while case "$fracpart" in
        0*[0-9]) fracpart=${fracpart:1};; # strip leading zero
        *) false;;
        esac ; do
        : # nothing
    done
}

# A shell function to compare two floating ppoint numbers for lessthan, 
# done entirely in BASH builtin so no subprocess needed

less_than () {
    split_int $1
    aint=$intpart
    afrac=$fracpart

    split_int $2
    
    # return code of the function is the return code of the last command
    [ $aint -lt $intpart -o $aint -eq $intpart -a $afrac -lt $fracpart ]
}
