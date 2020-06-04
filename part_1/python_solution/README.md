# Python solution

This version is a fairly straight-forward mapping of the C++ code to Python.  It will run under Python 2.7 or 3.6.  Output matches the C++ version for the 100k orders test.

Interestingly, it's quite a bit faster under python3.6 than python2.7 on my Mac Air.

Python 2.7:

    real	6m41.565s
    user	6m37.383s
    sys	0m1.236s

Python 3.6

    real	5m2.788s
    user	5m0.511s
    sys	0m0.620s