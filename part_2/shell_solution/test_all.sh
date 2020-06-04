#! /bin/sh

ret=0
./test_float_utils.sh || ret=1
./test_array.sh || ret=1

exit $ret
