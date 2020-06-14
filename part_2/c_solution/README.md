# C Solution

See the [Part 1 README](../../Part\ 1/c_solution/README.md) for notes on code style, building, testing and Unittest framework.

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

A smaller improvement is for each OrderList to remember what size was last sorted, and only sort when a sort is requested and the OrderList has had new elements added.   Have to hotwire some of the unit tests to know this.

This is a much bigger win than I expected (20%), and also prepares for some of the following optimisations.

    real	0m1.492s
    user	0m1.459s
    sys 	0m0.020s

## Hand-craft a stable insertion sort

The requirements for sorting the OrderList are fairly constrained:
- Sort must be stable, on price (ascending or descending)
- New elements are appended to the array,
- Elements are only ever popped off the front
- Elements are not modified (except the qty of the top of book, which does not affect the sorting)

These constraints mean once a set of Orders has been sorted, they don't need to be re-arranged, the only operation is new elements may be inserted in the middle.  Thus we don't need the full generality of `qsort()`, which has the following disadvantages for this purpose:
- It does not gain any benefit from the array being mostly-sorted, so runs in O(n log n) even on already-sorted input. (cf. the [Timsort](https://en.wikipedia.org/wiki/Timsort) used in Python which can sort nearly-sorted lists in O(n) time).  
- It is not stable by default, so we have to synthesise a generation number and include it in a compound sort key to ensure stability.  This increases the size of the Order object and leads to more data being moved around during the sort, as well as making the comparison function a bit slower.
- As a library routine, it is coded in terms of `void *` pointers and incurs a function call for each comparison.

These factors indicate that we can hand-code a stable insertion-sort, using inline comparisons and concrete types instead of function calls and `void *` pointers.  This is (as expedted) a big win, around 40%:

    real	0m0.832s
    user	0m0.793s
    sys 	0m0.020s

## Remove generation

Now that the sort is stable, we can remove all knowledge of generation numbers.  OrderBook needs to remember whether the last order was buy or sell so it can pick the correct price.  

This is a good win, more than I expected:

    real	0m0.556s
    user	0m0.528s
    sys 	0m0.015s

## Add Binary search to the insertion sort

The hand-crafted insertion sort uses a linear search to find the insertion point in the already-sorted list.   This makes the whole sort more or less equivalent to bubblesort, O(n^2). This can be improved to a binary search, at the cost of some code complexity, to make it O(n log n).   This is a bit complex to code, and corner cases hard to get right, but with some unit testing and deep thought this can work.

This is quite a win for the 100k case:

    real	0m0.331s
    user	0m0.309s
    sys 	0m0.014s

but there is still some O(n^2) behaviour somewhere:

    100k    0.34 real
    200k    1.17 real
    500k    6.91 real
    1m      31.57 real
    2m      125.32 real 

Quick look with the profiler says this is in `memmove()` in the sort functions.  The reason is that while the binary search reduced the *comparison* efficiency to O(n log n), there is still a `memmove()` operation of average size n/2 for each input line.  So the *move* efficiency is still O(n^2).  

Lesson here is that we need to consider both comparisons and move/swaps when evaluating algorithms.  In some contexts (such as this), move efficienty probably dominates and it's worth a few extra compares to eliminate a move.   In other contexts (e.g. in Python), moves are very cheap (pointer copies) but compares can be arbitarily expensive (calls to Pyhon code), so optimizing compare efficiency is most important.

## Remove the sort altogether

The moving of sorted items dominates runtime for larger input sizes.  But we don't *really* need the list sorted, all we need to know is the current top of the book, and a way to find the next top of the book once the current is removed.  We can eliminate all sorting of the Order data, leaving them in time-priority order, and keep note of the top of book as the order list is compiled.  This makes inserts O(1) complexity.   When we process a trade and remove the top of an order book, we need to search the list of orders in time order to find the next top-of-book.  This is an O(n) operation.   Overall, this is asymtotically O(n^2) but given that inserts dominate removes 25-1, this should be a win until input size gets very large.  Resulting code is a lot simpler as well.   The end result is that the order book is effectively a priority queue, with an odd implementation.

This is a good (50%) win for the 100k case:

    real	0m0.174s
    user	0m0.149s
    sys	    0m0.014s

And the scaling is good, the O(n^2) doesn't become significant until around 1-2 million orders, and even then it's way faster (10x) than the sort solution.

      100k orders:         0.17 real         0.14 user         0.00 sys
      200k orders:         0.37 real         0.33 user         0.01 sys
      500k orders:         1.20 real         1.15 user         0.02 sys
     1000k orders:         3.74 real         3.63 user         0.07 sys
     2000k orders:        13.82 real        13.19 user         0.18 sys
     5000k orders:        85.22 real        84.03 user         0.63 sys
    10000k orders:       356.67 real       353.27 user         1.69 sys

## hand-code OrderBook lookup

Every line of input calls `BookList_find()` to find the BookList for the current instrument.  So optimizing this function could be significant.  The standard libc doesn't have a hash table or any other useful data structure, so we keep the OrderBooks in a sorted list and look them up via `bsearch()`.  For the very few cases where this is a new instrument (couple of dozen out of millions of lookups) we use `qsort()` followed by another `bsearch()`.

As noted for `qsort()` above, `bsearch()` is written with `void *` and function call per comparison.  A hand-crafted binary search can improve on this in three ways:
 - Use concrete types and in-line compares
 - In the case of new instrument, we can use the bsearch to find the insertion point saving a sort and another lookup
  - Most usefully, the input data is quite bursty and the next order almost always has the same instrument as the previous order.  Stats says only 13k orders have different instrument to previous, out of 100k test file. We can cache the results of the last lookup and use that to start the search, avoiding many compares and finding the correct order book immediately in most cases.

This saves about a a second per million orders.  Thsi is a small win (few percent) for 100k case, but a good win on medium-sized inputs, before getting overwhelmed by the O(n^2) behaviour on the 10M case.

    real	0m0.172s
    user	0m0.151s
    sys	    0m0.014s

## Use a real priority queue / heap

The optimized C++ code uses a priority queue (implemented as a [heap](https://en.wikipedia.org/wiki/Heap_(data_structure)) from the C++ standard library) to keep the best-priced order at the top of the order book.  Heaps are O(log n) for both insert and pop, so total running time should be O(n log n).

The downside of the usual heap datastructure is that it is not stable, so we need to add a generation number back into the Order object in order to maintain the stable FIFO behaviour.  For a C programmer, there is no heap function in libc so it would have to be manually coded or downloaded from GitHub or something.  Heap algoithms for push() and pop() are pretty simple so hand-coding is not too hard, and will be more efficient than a library version.

Current sort-less solution is (amortized) O(1) for insert but O(n) for remove.  Given the workload is 95% inserts, it's not clear if the faster removes of a heap would offset the slower inserts, and/or at what input size those two curves would cross over.

This in the end is (as expected) the biggest win yet, especially on large inputs.  Even for the 100K case, it's about 20% faster:

    real	0m0.147s
    user	0m0.117s
    sys 	0m0.017s

For the longer tests, this is showng near-linear scaling, so a massive speedup:

      100k orders:         0.14 real         0.11 user         0.00 sys
      200k orders:         0.25 real         0.22 user         0.01 sys
      500k orders:         0.66 real         0.55 user         0.04 sys
     1000k orders:         1.35 real         1.11 user         0.08 sys
     2000k orders:         2.81 real         2.22 user         0.17 sys
     5000k orders:         6.64 real         5.58 user         0.44 sys
    10000k orders:        13.67 real        11.17 user         0.88 sys

Profiling shows that the libc functions used to read & parse each input order (`fgets()` and `sscanf()`) being ~80% of the run time (for the 10M case).  This (a) explains why this is now looking like O(n) solution, and (b) suggests future optimizations are going to get a *lot* harder.

As an aside: stable heap algorithms exist (see e.g. [this CSTheory post](https://cstheory.stackexchange.com/questions/593/is-there-a-stable-heap)) but they are complex to implement and/or use O(n) extra space, which is no better than the extra generation number in each order, so we've not bothered here.

## Use k-ary heap

Once we have a (usual binary) heap to store the order books, we can consider using a higher-order heap instead.  Insert into an k-ary heap is O(log_k n), with no change to code complexity, so for e.g. 4-ary heap insert has half the compare/swap operations as a binary heap.  Remove for k-ary heaps is also asymptotically O(log_k n) but the logic is more complex and the constant factors higher as k-ary remove has to compare all k children of the current node.  Given that inserts are far more common than removes, this is an acceptable tradeoff for this project.

If we use k-ary heap where k=2^*i* (i.e. k=4,8,16..) then this is simple to code and fast to calculate parent/child indexes. k-ary heaps are probably also kinder on caches and VM pagers as more of the accesses are closer together in memory.

This particular optimization is not available to C++ developers without writing their own k-ary heap container.  There is a very sophisticated C/C++ heap implementation on [GitHub](https://github.com/valyala/gheap) that has B-Heaps (which are a more VM-friendly heap), k-ary heaps and other cleverness.  For the C version, it is like `qsort()` and has function calls and `void *` pointers, so a hand-coded implementation may be faster.

Code changes to support k-ary keap is remarkably small.  For the smaller inputs, this does have a slight noticable increase in runtime, but for the 10M case, it has a positive impact:

| size | k=2        |      k=4   | k=8        |
| ---: | ---------: | ---------: | ---------: | 
| 100k | 0.12 real  | 0.13 real  | 0.15 real  |
| 200k | 0.25 real  | 0.28 real  | 0.25 real  |
| 500k | 0.60 real  | 0.59 real  | 0.62 real  |
| 1M   | 1.18 real  | 1.20 real  | 1.20 real  |
| 2M   | 2.55 real  | 2.42 real  | 2.54 real  |
| 5M   | 8.22 real  | 6.16 real  | 6.01 real  |
| 10M  | 13.67 real | 12.13 real | 12.08 real |


## Replace sscanf()

As noted above, runtime is now dominated by sscanf().  This can be replaced by explicit conversions using `strchr()`, `strtol()` and `strtod()`, at the cost of longer and uglier code in `main()`.  This is worth nearly 50% on the 100K test: 

    real	0m0.088s
    user	0m0.069s
    sys	    0m0.015s

and about 30% on the 10M test (10.9 vs 13.7 sec).

## Optimize swaps in heap sift up/down

The idea for this optimization came from the python `heapq.py` module, see  the comments in [The Python heapy.py file](https://github.com/python/cpython/blob/264e4fd9619dfab3d9de7f78a46efd8772b03ea6/Lib/heapq.py#L205). 

Current implementation of heap pop/insert does one swap for every compare.  Each swap is 3 moves of an Order struct, so relatively expensive.  End result is that a single record gets swapped multiple times per operation, until it is in the correct place.  Better to save the record in a tmp variable, find the correct place to insert it (and moving, not swapping, the other heap elements to maintain heap invariants), then insert the new record.  This cuts the amount of struct copies by a factor of (nearly) 3.  For `append()` this is a small code chamge, for `pop()` the inner lpop needs to be careful which elements it is comapring.

This is a small win, even given the runtime dominated by parsing the input:

  100k orders:         0.12 real         0.06 user         0.00 sys
  200k orders:         0.21 real         0.12 user         0.01 sys
  500k orders:         0.56 real         0.30 user         0.03 sys
 1000k orders:         1.04 real         0.60 user         0.07 sys
 2000k orders:         2.08 real         1.20 user         0.13 sys
 5000k orders:         5.28 real         3.01 user         0.33 sys
10000k orders:        10.58 real         6.02 user         0.65 sys

# Failed optimizations

## replace fgets()

Can we be faster by replacing the `fgets()` and parsing functions (`strtol()` etc) with `getchar()` and hand-coding the conversions to int/double?

Answer is no, and by a surprising margin.  Coded with plain `getchar()` this is ~3x as expensive as `fgets()`.  Most of this is in the locking underlying the `getchar()` call, replacing this with `getchar_unlocked()` is much better, but still ~10% slower than fgets.

# Possible future optimizations

## PriceStep object

Instead of maintaining a single OrderList per instrument/side, we can add another level of data structure.  The OrderBook would store lists of PriceStep objects, each of which contains a list of Orders for a given price, in time priority.  New orders added to the back, traded orders popped from the front of the PriceStep.  So there would be no need to sort the Orders at all, only the PriceStep objects, and then only when a new price is seen, which should cut the sorting (or heap operations) down to almost nothing.  Searching price steps can be done using binary search, which is O(log n) on the number of price steps per stock/side (not order). 

In addition, the Order objects wouldn't need the price element (or the generation element), making them smaller and more efficient.

Given that the priority queue has now improved the order push/pop so dramatically that the reading & parsing of input data is now the most expensive operation, this change may not have noticable improvement.

## match before push

Current implementation pushes new orders onto the OrderList then runs `match()` to see if there are any trades.  In the case where the order crosses the spread and produces a trade, it is likely that the order will then be immediately removed from the OrderList.  We could save some (probably insignificant) time by running the match as part od the append process and only pushing the order into the OrderList if it doesn't get consumed immediately.
