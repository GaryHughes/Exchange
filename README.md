
# Performance of Different Implementations

These are sample timings collected on a 2018 2.9 GHz Intel Core i9 MacBook Pro using the supplied input files orders-100K.txt and orders-10M.txt.

## Part 1

|    | 100K         |
|----|:------------:|
|C++ | 00:00:15.122 |
|Go  | 00:00:28.458 |
|F#  | 00:00:09.994 |
|C#  | 00:00:00:479 |

## Part 2

|    | 100K         | 10M          |
|----|:------------:|:------------:|
|C++ | 00:00:00.060 | 00:00:06.314 |
|Go  | 00:00:00.088 | 00:00:08.902 |
|F#  | 00:00:00.480 | 00:00:46.044 |

