# Python solution

## Python Version

First change is to run under python3, it's quite a bit faster under python3.6 than python2.7 on my Mac Air, by an astonishing 25%.  For the 100k orders test:

Python 2.7:

    real	6m41.565s
    user	6m37.383s
    sys	    0m1.236s

Python 3.6

    real	5m2.788s
    user	5m0.511s
    sys	    0m0.620s

## Don't copy Instrument

The `Order` type is carrying the Instrument, but doesn't need it, only use for this is in the OrderBook, which has it's own copy.  This does mean the split of the input line has to be hoisted to the main loop.

Saving is a few seconds only:

    real	4m58.007s
    user	4m54.015s
    sys 	0m0.623s

## Use `__slots__`

The python `__slots__` class variable replaces the usual dict in an object for instance variables with a fixed array.  This greatly reduces the flexibility of the users of the class, but can be much faster to look up attributes.  As the Trade and Order objects are heavily used and really only used as PODs, this is a price we can afford.  As an aside, instance variables defined in `__slots__` also clash with class variables, which is not true for normal objects.

This is a good win, over 15%:

    real	4m14.815s
    user	4m10.738s
    sys 	0m0.482s

## Omit the generation

Python sort routine is both stable and very well optimised for nearly-sorted data, so there is no need to rely on generation number.  The OrderBook object can keep the latest added order to know which price to use. As we are relying on the stable sort, we can also change the `match()` code to expect the top of the order book to be the start of the array, which means we can append new orders into the *end* of the array.  This is slightly cheaper and the more common operation, so small extra saving there.

This is a massive win, saving more than 50%. This is much more than I expected.  Some of this improvement can be attributed to the use of plain floats rather than tuples as the key to `sort()` (~50 sec on this test).

    real	1m56.511s
    user	1m54.546s
    sys     0m0.301s

## Lazy sort

Sorting is clearly the expensive bit of the problem.  The OrderBook object could keep track of best bid & offer manually, and only bother sorting the arrays when running the match() and the prices are known to overlap.

Given how well the Python [Timsort](https://en.wikipedia.org/wiki/Timsort) implementation handles nearly-sorted data (close to linear time), this might not be the win for Python that it might be for other environments that use more naive and general sort.

As it turns out, this is by far the biggest optimization of all.  Amazing 95% improvement:

    real	0m6.895s
    user	0m6.685s
    sys     0m0.082s

That's 1/60th the time of the original version.... 

# Failed optimizations

## Compare Orders directly
Sorting the order book is clearly an expensive part of the process, given the benefit of omitting the generation and simplifying the sort.  Can we simplify the sort even more by making the Order objects diurectly comparible?

Turns out no, this is about 40% more expensive.

# Possible further optimizations

## Use Tuples for Orders

Changing the Order objects to use `__slots__` was a reasonable win.  Given there is no methods on these objects, could replace them with tuples which should have even faster element lookup.  

This will also sort naturally without needing a key function which might save even more time; try with natural sorting based on tuple, or with a key function to extract the price from the tuple.

