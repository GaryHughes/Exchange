#! /bin/sh

# Somecode to test simulated associative arrays

. unittest.sh
. array_utils.sh


set_array_key a BHP 12345
ensure_true [ ${!a} = 12345 ]

# check that init doesn't overwrite existing key 
set_array_key a BHP 23
ensure_true [ ${!a} = 12345 ]

# setting works
set_array_value a 3.45
ensure_true [ ${!a} = 3.45 ]

# Doesn't have to be a number
set_array_value a foo
ensure_true [ ${!a} = foo ]

# try multiple keys
set_array_key a AAA bar
ensure_true [ ${!a} = bar ]
# and doesnt overwrite previous key
set_array_key a BHP
ensure_true [ ${!a} = foo ]

# Can have multiple independent arrays 
set_array_key b BHP baz
ensure_true [ ${!b} = baz ]
# and doesnt overwite other array
ensure_true [ ${!a} = foo ]

process_test_results
