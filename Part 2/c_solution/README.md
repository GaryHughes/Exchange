# C Solution

See the (Part 1 README)[../../Part\ 1/c_solution/README.md] for notes on code style, building, testing and Unittest framework.

## Performance Notes

All performance measurements are median-of-3 on the 100k order test on a 2010-vintage Mac Air.

Performance of the base Part 1 version is pretty poor, around 23 seconds:

    real	0m22.949s
    user	0m22.794s
    sys	    0m0.061s

Profiling says this is 99% in `qsort()`.

# Optimizations

## Remove Instrument and Side from Order

Order object doesn't need to carry around the instrument or the side, this is in the OrderBook.  Smaller Order objects mean less data for qsort() to move, so a small win to be had there.  Implementing this means hoisting the code to split input lines into `main()`, and logic for buy/sell and generation into `OrderBook_append()`. These changes also makes other optimizations easier. This turns the Order object into a simple container with no associated logic and no unit tests, so subsume this back into `exchange_defs.h`

Similarly we can reduce the size of the ID to 4 bytes which will have similar (small) effect on `qsort()` time.

This is worth a few percent:

    real	0m21.512s
    user	0m21.396s
    sys	    0m0.
    
## Lazy Sorting

Currently, the OrderList gets sorted on every input line, which is where most of the CPU is going.  We can avoid this by keeping the best bid and best offer in the OrderBook object and only sorting the OrderLists if we have an overlap and need to generate a trade.

As expected, given how `qsort()` dominates the runtime, this is a huge win:

    real	0m1.715s
    user	0m1.683s
    sys 	0m0.020s

## Lazier sorting 

A smaller improvement is for each OrderList to remember what size was last sorted, and only sort when a sort is needed and the OrderList has had new elements added.   Have to hotwire some of the unit tests to know this.

This is a much bigger win than I expected (20%), and also prepares for some of the following optimisations.

    real	0m1.492s
    user	0m1.459s
    sys 	0m0.020s

# Possible future optimizations

## Hand-craft a stable insertion sort

The requirements for soring the OrderList are fairly constrained:
- Sort must be stable, on price (ascending or descending)
- New elements are appended to the array,
- Elements are only ever popped off the front
- Elements are not modified (except the qty of the top of book, which does not affect the sorting)

These constraints mean once a set of Orders has been sorted, they don't need to be re-arranged, the only operation is new elements may be inserted in the middle.  Thus we don't need the full generality of `qsort()`, which has the following disadvantages for this purpose:
- It does not gain any benefit from the array being mostly-sorted (cf. the [Timsort](https://en.wikipedia.org/wiki/Timsort) used in Python which can sort nearly-sorted lists in O(n) time).  
- It is not stable by default, so we have to synthesise a generation number and include it in a compound sort key to ensure stability.  This increases the size of the Order object and leads to more data being moved around during the sort, as well as making the comparison function a bit slower.
- As a library routine, it is coded in terms of `void *` pointers and incurs a function call for each comparison.

These factors indicate that we can hand-code a stable insertion-sort, using inline comparisons and concrete types instead of function calls and `void *` pointers.  Then we can remove all knowledge of generation numbers. This should be a big win.