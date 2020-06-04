#! /bin/sh

# some utilities to simulate a dict / associative array using a 
# heap of normal shell variables.  

# Requirement is no subshells, subcommands, backticks &c.

# usage pattern is
# - set_array_key first to establish the current "key" for the 
#   array.  An optional initializer can be specified
# - set_array_value to set the value for that array/key
# ${!arrayname} will read the current value

set_array_key () {
    # $1 = "array" name
    # $2 = "key"
    # $3 = initial value (optional)
    name=$1_$2
    eval $1=$name
    [ -z "${!name}" -a ! -z "$3" ] && eval $name="$3"
    : # return true
}
set_array_value () {
    # $1 = array name
    # key is assumed from set_array_key
    # $2 = value
    eval ${!1}="$2"
}
