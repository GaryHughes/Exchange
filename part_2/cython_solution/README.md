# Cython solution

This version starts with the optimized Python implementation from [part_2/python_solution](../python_solution/README.md), not the Part 1 cython solution.

First step is to run the Python code as a compiled Cython extension, then proceed to use the various cython facilities (typing, C structs &c) to improve performance.

# Building

### Python versions

The extension API for python2 & python3 are not the same, so a cython extension built with python3 will not load on python2.  If you see errors on import like

    Symbol not found: _PyMethodDescr_Type

then double-check you are using the same python version.  We'll be using 3.6 for this purpose.

### Cython install

Cython is installed using pip, so something like

    pip3 install cython

### Build the extension

This is easy, using setup.py:

    python3 setup.py build_ext --inplace

### Running unit tests

This is simple:

    python3 -m unittest discover


# Performance

All the below timings are median-of-3 run against the 100K orders test on a 2010-vintage Mac Air.  All match the output of the C++ reference version.

The optimised Python3 version runs the 100K test in about 6 seconds:

    real	0m6.468s
    user	0m6.227s
    sys	    0m0.121s

Simply compiling the same Python3 code in Cython reduces this by a respectable 35%:

    real	0m4.208s
    user	0m4.030s
    sys	    0m0.074s

# Optimizations

Changing Order object to use `__slots__` was a big win, so first change is to make the Order object a cython `cdef class` so that the cython code can call using C types and calling conventions.  This requires changing the API for the class slightly, but that's an easy change. This saves about 10%:

    real	0m3.839s
    user	0m3.706s
    sys 	0m0.057s

Same change for the Trade type has no measurable impact.

Cython-ising the `match()` function and declaring the local variables with C type is a small win:

    real	0m3.656s
    user	0m3.547s
    sys	    0m0.059s

# Remove Python list type

The current version uses Python wrappers around the Order struct, stored in Python list objects.   This makes it easy to manipulate list contents, use existing Python stable `sort()`, and write the unit tests in Python.  However this code is in the critical path and executed for every input line.There is potentially a big gain in hiding the Order type inside the OrderBook and making it a `cdef struct` stored in a C array.  However, this is much more complex code, as the Cython code will have to manage memory, implement a stable sort, and expose the C struct for unit testing.  And the code is a bit ugly as the C code is not object-oriented so you have to write stand-alone functions not member functions.

The standard `qsort()` routine in libc is not stable, so we have to re-add the generation number to the `Order` object to get stable sorting.  Unit-testing is enabled by some special member functions on the OrderBook object that can access the C structs and return to Python for the unittests.  These extra functions have no performance impact.

The hardest bit is to extract the buyer/seller IDs from Python into the C struct, in a manner that allows the Order objects to be copied around while sorting, and then converting the IDs back into Python string types when required.  This takes a bit of Unicode encoding/decoding, and in order to make the Order struct small, we only allow max 8 characters in the buyer ID.

This is a good win, about 30%:

    real	0m2.133s
    user	0m2.032s
    sys	    0m0.052s

## Use a priority queue

As with the other solutions, there is a large gain from using a more appropriate data structure.  Changing from sorted list to a priority queue is a big win for smaller runs (4x for the 100K case) and overwhelming win for the longer runs.  

Behaviour is now more or less linear, which says remaining bottlenecks are in the reading and parsing of the input data.

      100k orders:         0.52 real         0.38 user         0.08 sys
      200k orders:         0.78 real         0.71 user         0.05 sys
      500k orders:         1.79 real         1.68 user         0.09 sys
     1000k orders:         3.53 real         3.29 user         0.17 sys
     2000k orders:         7.04 real         6.52 user         0.31 sys
     5000k orders:        17.10 real        16.23 user         0.72 sys
    10000k orders:        34.52 real        32.41 user         1.58 sys

# Possible further optimizations

## Investigate Unicode IDs
A fair bit of the effort reading each input line is converting the Python unicode string objects for buyer/seller IDs into char array in the Order struct.  There may be a better/faster way to do this 

## Consider k-ary heap

Priority queue implementation is based on a standard binary heap.  The C solution benefited from a move to 4-heap due to the excess of push operations over pop operations offsetting the more complicated pop logic.  The cython version would also likely gain a small amount.
