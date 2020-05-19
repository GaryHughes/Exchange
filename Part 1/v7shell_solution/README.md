# Unix Version 7 shell

An implementation in Unix Version 7 Bourne shell (& utilities).  Because we can.  And to prove that 40-year-old systems (and the developers that used them) are still useful.

The expectation is that this script would run on a 1980-vintage Unix Version 7 system on a PDP 11 (16-bit, 64k memory), using only the built-in text utilities from V7 Unix.  This runs on a modern (MacOS) BASH shell, but I've tried hard to limit the code to features documented in the V7 man pages.  However, at this point I have not checked it on an actual PDP11/V7 emulator, so there may be something I've overlooked.  

See [Dennis Ritchie's archive of the V7 documents](https://s3.amazonaws.com/plan9-bell-labs/7thEdMan/bswv7.html) for the V7 manuals, and the [Wikipedia entry on Bourne Shell](https://en.wikipedia.org/wiki/Bourne_shell) for some historical notes on when features were added. 

We use the following V7 utilities:
 * echo (Yes, in V7 this was an external utility `/bin/echo`, not added as a shell builtin until SVR2 in 1984)
 * sed 
 * awk (mainly to count line numbers and handle floating point comparisons)
 * head (can be simulated with `sed 1q`)
 * sort (and sort -r)
 * test
   * This is also not builtin until System III
   * `case ... esac` *was* built-in and available in V7, and can be used to more efficiently implement some comparisons as it uses fileglob matching on strings.  This was a fairly widely-used optimization and well known to shellscript authors.
 * expr (not sure when this became a built-in)
 * mv & rm for file manipulation

 To keep it to V7 standards, we do *not* use the following features of modern (bash) shells:
  * `#` for comments. these were added to Unix System III in 1981. V7 scripts used `:` command with quoted string and/or here-documents instead, if they used comments at all.
  * Shell functions were only available from SVR2 shell in 1984.  Use cut-n-paste or explicit shell scripts.
  * the `[` alias for `/bin/test`.  I'm not sure when this was added.
  * `$((...))` arithmetic expansion which is a relatively recent `bash` feature
  * similar built-in arithmetic features (`if ((expression))` etc)
  * `$(..)` for command substitution

## Data Format

All data is stored in the filesystem in a temporary sub-directory of the current directory.  For each symbol, there is a file each of buys and sells, sorted so that the top of the order book is the first line in the file.  If there are no orders for that symbol/side, there will be no file. Format of the files is one line per order; fields are space-separated for compatibility with the usual Unix utilities.  Fields are:
* price
* lineno
* user
* qty

The sell file can be sorted with `sort -n +0 -1 +1 -2`.  Need to have 2 separate fields here, as the logic for numeric comparisons will implicitly ignore the second field. This was the source of a subtle and hard-to-find bug.  Fortunately, `sort -n` does handle real numbers.
For buy orders, need to reverse the price comparison so `sort -n +0r -1 +1 -2` 

The biggest challenge is comparing prices, which in the sample data are real numbers, but not formatted with a fixed number of decimals.  `/bin/test` only supports integers, and string comparison does not work for less-than in this case. The `awk` utility supports arithmetic on real mumbers so that can be used indirectly with a bit of messing about and some ugly quoting.

One problem of using the filesystem as the database is that *some* symbols in the input files have `/` character in them, which is significant in filenames.  Converting the `/` to `~` (or some other character that's not in the normal input files) on the input and back on the output fixes it; this can be done with 2 extra `sed` processes.  (Not widely-known hint:  you can redirect stdin/stdout and use pipes with `while ... do ... done` loops!)

## Testing

Hah.  80's programmers dont write test cases.  But it does get the same output as the other implementations on the 100K order case.


## Performance

As you might expect, this is *sloooow*.  Nearly four hours to complete the 100k order case on my 2010-vintage Macbook:

    real	227m28.877s
    user	84m18.340s
    sys   24m23.820s

So this version will not be added to the automatic benchmarking......

For 1000 orders it's about 25 seconds elapsed.