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

    100k    0.17 real
    200k    0.37 real
    500k    1.38 real
    1m      4.59 real
    2m      14.05 real
    5m      94.23 real
    10m     380.79 real

# Possible future optimizations

## Use a real priority queue / heap

The optimized C++ code uses a priority queue (implemented as a [heap](https://en.wikipedia.org/wiki/Heap_(data_structure)) from the C++ standard library) to keep the best-priced order at the top of the order book.  Heaps are O(log n) for both insert and pop, so total running time should be O(n log n).

The downside of the usual heap datastructure is that it is not stable, so we'd need to add a generation number back into the Order object in order to maintain the stable FIFO behaviour.  For a C programmer, there is no heap function in libc so it would have to be manually coded.  This is not too hard.

Current sort-less solution is (amortized) O(1) for insert but O(n) for remove.  Given the workload is 95% inserts, it's not clear if the faster removes of a heap would offset the slower inserts, and/or at what input size those two curves would cross over.

Stable heap algorithms exist (see e.g. [this CSTheory post](https://cstheory.stackexchange.com/questions/593/is-there-a-stable-heap) but they are complex to implement and/or use O(n) extra space, which is no better than the extra generation number in each order.

## Use k-ary heap

Once we have a (usual binary) heap to store the order books, we can consider using a higher-order heap instead.  Insert into an k-ary heap is O(log_k n), with no change to code complexity, so for e.g. 4-ary heap insert has half the compare/swap operations as a binary heap.  Remove for k-ary heaps is also asymptotically O(log_k n) but the logic is more complex and the constant factors higher as k-ary remove has to compare all k children of the current node.  Given that inserts are far more common than removes, this is an acceptable tradeoff for this project.

If we use k-ary heap where k=2^*i* (i.e. k=4,8,16..) then this is simple to code and fast to calculate parent/child indexes. k-ary heaps are probably also kinder on caches and VM pagers as more of the accesses are closer together in memory.

This particular optimization is not available to C++ developers without writing their own k-ary heap container!

## PriceStep object

Instead of maintaining a single OrderList per instrument/side, we can add another level of data structure.  The OrderBook would store lists of PriceStep objects, each of which contains a list of Orders for a given price, in time priority.  New orders added to the back, traded orders popped from the front of the PriceStep.  So there would be no need to sort the Orders at all, only the PriceStep objects, and then only when a new price is seen, which should cut the sorting down to almost nothing.  Searching price steps can be done using binary search, which is O(log n) on the number of price steps per stock/side (not order). 

In addition, the Order objects wouldn't need the price element, making them smaller and more efficient.
