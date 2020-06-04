# Exchange

# Part 2

Benchmark your program against the supplied file orders-100K.txt, how long does it take to run? 

* Record the conditions under which you run your tests
    * Hardware
        * CPU
        * Memory
        * HDD/SSD
    * Operating System
    * Language/Compiler version and any optimisations you've applied.
    * Ensure there is no significant load on the system from other processes
* Run this test multiple times and record the min, max and average.
        
        $ time cat orders_100K.txt | ./Exchange > trades.txt
        real	1m14.544s
        user	1m14.524s
        sys	0m0.012s

* How much faster can you make it?
* What did you change and why?
