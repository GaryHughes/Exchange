# C Solution

This version is a fairly direct version based on the C++ implementation, done in standard C11.

There is more code and more objects implemented in this C version than the C++ version as the C++ version uses standard container classes which the C author has to implement by hand.

## Building

A `Makefile` is provided.  Apart from a C compiler there are no other build/test dependencies.  Development was done on MacOs and cross-checked for portability on FreeBSD. (BSD make is *not* GNU make!)

## Performance

Performance is pretty poor, around 23 seconds on my old Mac Air.

    real	0m22.949s
    user	0m22.794s
    sys	    0m0.061s

Bit of quick profiling (on FreeBSD, haven't worked out how to do command-line profiling on MacOs as gprof is not supported) shows 99% of the time is in qsort() and the comparison functions, sorting the OrderList objects.  This is not unexpected.

## Some notes on code style

Unlike a lot of C code, this is written in a faux-OO style.  Types are all CamelCase without struct tags.  "Member" functions are distinguished by name conventions, *Type*_*membername*().  First argument is a pointer to the Type object (think `self` in python).

1 header and 1 implementation file per object type.

Some conventional member names for an example Type:
 - `Type * Type_init(Type *,..)` -  initialise an object that has already been allocated (automatic variable, array member &c), equivalent to C++ constructor.  Return a pointer to the initialised object or NULL if initialisation fails for some reason.
 - `void Type_destroy(Type *)` - Destructor, release any resources
 
Dynamic allocation is minimised if possible, as is structure passing/return, for performance reasons.  So the idiom is usually for the caller to allocate an object on the stack for the "return value" from the callee, and pass a pointer to the called function.  This idiom is also thread-safe, not that it matters for this project.

 ## Some notes on unit testing

 I've used [Unity](https://github.com/ThrowTheSwitch/Unity.git) as a unit test framework. It's mainly aimed at small/embedded C testing but works fine for this purpose.  It's fairly light weight (2 headers and a .c file), and seems to be actively maintained (last commit was a few days ago).
 
 As an experiment I've tried referencing it via `git subtree` feature:

    git subtree add --prefix  Part\ 1/c_solution/unity https://github.com/ThrowTheSwitch/Unity.git master --squash
 
This is a new one for me, and I have no real feel how this will work out in practice in a GitHub project.  It's also probably more expensive on resources compared to just copying 3 files (plus a LICENCE.txt) into the project.

## On the wisdom of floating point equality

Conventional wisdom says never compare floating point values for equality, as rounding may cause things that *ought* to be equal to not be so:

    $ python -c 'print float("1.1") * float("1.1") == float("1.21")'
    False
    $ python -c 'print float("1.1") * float("1.1") - float("1.21")'
    2.22044604925e-16

So the usual advice is that instead of:

    if (a == b) { ... }

you should do something like:

    if (fabs(a - b) < *epsilon*) { ... }

In general, this is very wise advice.  But it is cumbersome, somewhat expensive, and leaves the difficult and subtle open question of exactly what you should use for *epsilon* .... and a wrong choice may be worse than the original compare problem.

 For this project, we don't do any floating point *arithmetic*, all we do is convert from string to double (in `sscanf()`), copy the double values around for a bit, then print out (in `printf()`).  So there is no place where any precision loss can occur.   So long as `sscanf()` converts the same string representation  into the same double value (and it bloody well should!), we are OK to compare for equal.
 