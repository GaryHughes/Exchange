
# Performance of Different Implementations

These are sample timings collected on a 2018 2.9 GHz Intel Core i9 MacBook Pro using the supplied input files orders-100K.txt and orders-10M.txt.

## Part 1

|    | 100K         | 
|----|:------------:|
|C++ | 00:01:18.336 |
|Go  | 00:00:08.013 |
|F#  | 00:00:06.387 |

## Part 2

|    | 100K         | 10M          |
|----|:------------:|:------------:|
|C++ | 00:00:00.060 | 00:00:06.314 |
|Go  | 00:00:00.088 | 00:00:08.902 |
|F#  | 00:00:00.480 | 00:00:46.044 |

