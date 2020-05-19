#! /bin/sh

# some unit tests for the floating point manipulation

. float_utils.sh
. unittest.sh

ret=0

test_split_int () {
    # ensure $1 splits into $2 & $3
    split_int $1

    if [ "$2" -eq $intpart -a "$3" -eq $fracpart ]; then
        echo ".\c"
    else
        ret=1
        echo "Test failed: $1 -> $intpart $fracpart expected $2 $3"
    fi
}


test_split_int 0 0 0
test_split_int 33 33 0
test_split_int .10 0 100000
test_split_int 1.0 1 0
test_split_int 1.001 1 1000
test_split_int 99.9999 99 999900
test_split_int 999.9999 999 999900

ensure_exits_fail split_int foo
ensure_exits_fail split_int 23b
ensure_exits_fail split_int ""
# limits, only 6dp supported
ensure_exits_fail split_int 1.1234567

ensure_false less_than 1.1 1.1
ensure_false less_than 1.1 1.0 
ensure_true less_than 1.0 1.00001
ensure_false less_than 1.001 1.00001 
ensure_true less_than 1.009 1.01
ensure_false less_than 10.1001 10.100001 

process_test_results
