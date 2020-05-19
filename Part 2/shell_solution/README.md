# Shell Solution

This is based on the logic and design of the v7shell_solution from Part 1, but updated to modern BASH shell facilities, and whichever further optimizations make sense.

## Some notes on testing and timing

Given that the shell solution is slow (4 hours for the 100K orders), this version was developed using the first 2000 orders from that file:

        sed 2000q < ../../orders_100K.txt | time sh Exchange.sh

Base running time for the original version is 50 seconds:

        50.54 real        15.76 user        22.80 sys
        50.73 real        15.78 user        22.78 sys
        50.09 real        15.75 user        22.70 sys


Downside of this test is it has only 16 actual trades from 2000 orders, so the trade matching prcoess is not heavily exercised.  On the other hand, the full data set only has trades from about 4% of the orders so this shorter test is probably over-representing the trade match logic.

[Insert comment here about the evils of Flash Boys ruining markets with excessive order counts.....]

The base version is still much faster than the v7 verison would be, even after accounting for 40 years of hardware progress.  This is because the the test was run not with the V7 shell but with modern MacOS `bash`, which has builtin `expr`, `test` and `echo` functions, which are effectively free in bash but would require new processes `fork()/exec()` on a V7 system.

As is clear from the timing, this program is not CPU limited (4 hrs run time, less than 2 hours CPU) but is likely disk limited.  This is driven both by the actual file operations and also the I/O from the many processes created.  

So the focus of optimization is two-fold:
 * Primarily, to reduce the number of processes created, either by command substitution or explicit script action
 * Reduce the amount of file copying/renaming, or the cost of such operations
 
 As noted above, only a few percent of the orders give rise to trades, so the major wins would come from optimising the new order handling path and the no-trade-matching path; optimizing the handling of matched trades is unlikely to be noticable.

 Improvements to the actual shell script (by reducing variable usage, or similar) are also not likely to have material impact.

## Stylistic changes

The first set of changes update the script to modern standards.  This will likely have no measurable impact on performance, but will do much for readability, code reliability and possibly even allow for some automated testing.

* Use # comments and comment the code mode comprehensively
* Use [] for test
* Use shell functions for common operations
* Put the shell functions into an external file so they can be tested

After these changes, performance was not altered, as expected.

        50.51 real        15.65 user        22.70 sys


## Optimizations

### /tmp directory

Most modern Unix-like systems (including MacOS) have a special filesystem for `/tmp` that is some equivalent of a RAM disk - file operations such as `mv` or `rm` are much faster as they do not have to synchronously write to disk.  This script is very heavy on file operations so it seems that moving to `/tmp` would be an easy (2 line change) win.

And so it is, about 10% faster in elapsed time (but total CPU time is not much affected):

       44.10 real        15.51 user        21.57 sys
       44.37 real        15.58 user        21.69 sys
       43.94 real        15.53 user        21.51 sys

For MacOS at least, this also has the beneficial side-effect that the Searchlight indexer daemon `mdworker` ignores `/tmp` directory, so the script is not competing against `mdworker` for system resources.  `mdworker` was pretty active during the sript run, trying to madly index the rapidly-changing temporary files.

A related optimization of changing the PWD for the script to the temp directory and using relative paths in the current directory (another 2 line change) should theoretically be more efficient.  In practice, this is not measurably better

### Avoiding expr command substitution

Arithmetic in classic shell is done by running the `expr` utility and using command substituion (backticks).  In modern shells, the `expr` function itself is built-in, but the command substitution using backtic still costs a `fork()` for new process - cheaper than `fork()/exec()` that V7 shell would need for external commands, but still not cheap.  Bash has *arithmetic substitution* using `$((..))` which can do (integer) expressions without creating a new process.  We can use this where possible, particularly in the order-entry flow for fixing the negative qty in the sell order which is executed on about half the input lines.  

This turns out to be a pretty big win, taking another 10% or so off elapsed time and CPU time:

       37.76 real        13.73 user        18.53 sys
       38.41 real        13.75 user        18.62 sys
       38.21 real        13.72 user        18.54 sys

### Price comparisons

The next pain point is comparing the best buy & sell prices after each order is read to consider if the price overlaps and hence a trade is to be generated.  This code is run for every input line so savings here could be significant.  The existing solution runs an `awk` job on every line which is expensive.  Modern bash has several variable interpolation and string manipuilation features we can use to split a real number into an integer and fractional part.  If we assume a fixed maximum number of decimal places in the input data (say 6dp) and a resonable maxium number of digits in the integer part (say, 6), we can split the real number into integer and 
millionths and use built-in integer comparisons and save the call to awk.  If I could work out a way to do index() this would be neater code and less subject to arbitary limitations.  See `float_utils.sh` and related tests.

This is the biggest win yet, taking 25% off elapsed and 30% off CPU time:

       27.75 real        11.08 user        12.76 sys
       27.36 real        11.00 user        12.73 sys
       27.00 real        10.95 user        12.60 sys



# Trivial optimizations

The following optimizations are theoretically more efficient, but do not make any measurable difference.  They are included only because they make some of the other below potential optimizations more convenient.

## Handle `/` in symbol name

Currently done in a pair of sed scripts on input and output.   Can be handled with `${parameter/pattern/string}` in the main loop.

## Leave empty files alone

It's not necessary to remove empty orderbook files, as the test `[ -s filename ]` gives the same result (for the same cost) on empty files and missing files.  So we can save the occasional `rm` call.  These only happen when generating a trade, and usually not then, so not going to be material savings.

## Modify in-place

When processing a trade, it is sometimes required to reduce the quantity of the top order in the book rather than remove it.  This can be done with a simple sed rather than sorting every time.  Savings are minimal because this operation happens only rarely, and`sed` is only marginally faster than `sort`. But we include this optimization because it make it possible to eliminate the generation number.

# Current state

With the above optimizations, running on the 100k test orders, run time is now

     5892.47 real      4543.88 user       949.41 sys

or approximately 1hr 40m elapsed time.  This is an impressive 64% reduction in elapsed time, and about 15% reduction in total CPU time.  Output from this run matches the C++ reference implementation.

# Failed optimizations

### de-awk
Tried removing the awk feeding the main `while  read...` loop and using shell arithmetic to calculate the generation number and to skip blank lines.  The thought was this would avoid double-splitting each line (once in the awk and once in the read), but it turned out this was a couple of percent slower.

### Remove the mv

Most of the file operations would conceptually operate in-place (i.e. output filename same as input filename).  Alas, Unix utilities are filters and don't work that way, so most of the file operations look like

    sed ... $fname > fname.new
    mv $fname.new $fname

I.e. an extra move operation to rename the file. This is an extra process and occurs once or more per order.

It's possible to write a shell function to *copy* a file for the simple case of 1 source and 1 destination regular file:

    while read line ; do echo $line ; done < fromfile > tofile

For small-ish files it doesn't look too inefficient, and the hypothesis is that such a copy is faster than a new `mv` process.  For our purposes we don't care if superfluous `.new` files are left around, they'll be overwritten soon enough.

It turns out in practice that forking a plain `mv` process is a fair bit faster than shell copy - the shell copy version was 15% slower on the 2000-line test.  Difference on the larger tests (where the average size of the orderbook files being moved/copied is proportionately larger) would be even worse.

# Possible further optimizations

These look like they would be beneficial, but have not yet been investigated and implemented.

## Omit the generation number

 The `sort` utility is stable so there is no need to carry around a generation number and use that in the sort to provide stability.  3 elements to implementing this:
  - ensure all new orders are appended to the end of the book file
  - sort numeric by the first field in the file (price).  Numeric sorting implictly ignores anything after the first space, so there is no need to use the sort field selection (`+0 -1 +1 -2`), which probably makes the sort a bit faster.
  - when processing a trade that part-fills an order, amend the first line in-place (rather than at appending to the back and re-sorting, as the initial implementation did).

This would reduce the amount of data by maybe 20%, which should improve general performance. 

## Lazy sort

Given the staility of the `sort` utility, and the fact that only 5% of the orders create an overlapped market and change the best bid/offer, we could store the best bid/offer value in the script and only go to the file when the BBO changes.  This means the main new-order path would append only and not need to sort 95% of the time.  This is complex to implement in shell, as you would need to keep a price per symbol/side.  A possible approach is to use the bash indirect variables `${!varname}` to simulate a dictionary with many specially-named shell variables (or possibly some arrays).  

Potential saving here is quite large, possibly 90% or more, as the common path of new order and no trade would involve only appending a line to a file and require no sub-processes.

## Pingpong filenames

To remove the need for mv (or shell_copy) after every sort or update, we could use two file names for each order book and pingpong between the two as the "current" file.  This would require keeping state per symbol/side (probably using indirect variables again) to know which of the pingpong filenames was current.