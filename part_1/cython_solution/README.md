# Cython version

This simple version uses the exact same python file as the python_solution (via a symlink, good luck if you are using git on Windows!) but compiles it with cython.

Build it like this:

    python setup.py build_ext --inplace

This should create Exchange.so in the current directory.

Then run it using the usual runner:

    time ./runner < ../../orders_100K.txt >100.out 

Speedup is not insignificant 15% over the raw interpreted Python version:

Python3:

    real	5m38.245s
    user	5m27.098s
    sys	    0m1.105s

Cython:

    real	4m49.865s
    user	4m35.056s
    sys	    0m1.143s