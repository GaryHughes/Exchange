#! /bin/sh

# some unittest framwrok form shell (!!!)

ret=0

ensure_exits_fail () {
    # check that command exits with non-zero return code
    ("$@") 2>/dev/null
    if [ $? -ne 0 ]; then 
        echo ".\c"
    else
        echo "Test failed; Expected command $@ to exit with fail code but it didn't"
        ret=1
    fi
}

ensure_true () {
    # check that command returns true in the appropriate case
    eval "$@"
    if [ $? -eq 0 ]; then 
        echo ".\c"
    else
        echo "Test failed; Expected command $@ to succeed but it didn't"
        ret=1
    fi
}

ensure_false () {
    # check that command returns false in the appropriate case
    eval "$@"
    if [ $? -ne 0 ]; then 
        echo ".\c"
    else
        echo "Test failed; Expected command $@ to fail but it didn't"
        ret=1
    fi
}

process_test_results () {
    if [ $ret -ne 0 ]; then
        echo
        echo "Some tests failed!"
    else
        echo " OK."
    fi
    exit $ret
}