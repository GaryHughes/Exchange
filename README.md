| Language |     Part 1    |     Part 2    | 
|:--------:|:-------------:|:-------------:|
|  C++     |               |               | 
|  C#      | [![Build Status](https://dev.azure.com/garyedwardhughes/Exchange/_apis/build/status/Part%201%20-%20C%23?branchName=master)](https://dev.azure.com/garyedwardhughes/Exchange/_build/latest?definitionId=4&branchName=master)              | [![Build Status](https://dev.azure.com/garyedwardhughes/Exchange/_apis/build/status/Part%202%20-%20C%23?branchName=master)](https://dev.azure.com/garyedwardhughes/Exchange/_build/latest?definitionId=6&branchName=master) | 
|  F#      | [![Build Status](https://dev.azure.com/garyedwardhughes/Exchange/_apis/build/status/Part%201%20-%20F%23?branchName=master)](https://dev.azure.com/garyedwardhughes/Exchange/_build/latest?definitionId=5&branchName=master)              | [![Build Status](https://dev.azure.com/garyedwardhughes/Exchange/_apis/build/status/Part%202%20-%20F%23?branchName=master)](https://dev.azure.com/garyedwardhughes/Exchange/_build/latest?definitionId=7&branchName=master) |
|  Go      | [![Build Status](https://dev.azure.com/garyedwardhughes/Exchange/_apis/build/status/Part%201%20-%20Go?branchName=master)](https://dev.azure.com/garyedwardhughes/Exchange/_build/latest?definitionId=8&branchName=master)              | [![Build Status](https://dev.azure.com/garyedwardhughes/Exchange/_apis/build/status/Part%202%20-%20Go?branchName=master)](https://dev.azure.com/garyedwardhughes/Exchange/_build/latest?definitionId=9&branchName=master) |
|  Rust    | [![Build Status](https://dev.azure.com/garyedwardhughes/Exchange/_apis/build/status/Part%201%20-%20Rust?branchName=master)](https://dev.azure.com/garyedwardhughes/Exchange/_build/latest?definitionId=10&branchName=master)              | [![Build Status](https://dev.azure.com/garyedwardhughes/Exchange/_apis/build/status/Part%202%20-%20Rust?branchName=master)](https://dev.azure.com/garyedwardhughes/Exchange/_build/latest?definitionId=11&branchName=master)              |


# Performance of Different Implementations

These are sample timings collected on a 2018 2.9 GHz Intel Core i9 MacBook Pro using the supplied input files orders-100K.txt and orders-10M.txt.

## Part 1

|     | 100K         |
|-----|:------------:|
|C++  | 00:00:15.122 |
|Go   | 00:00:28.458 |
|F#   | 00:00:09.994 |
|C#   | 00:00:00:479 |
|rust | 00:00:02:260 |

## Part 2

|     | 100K         | 10M          |
|-----|:------------:|:------------:|
|C++  | 00:00:00.060 | 00:00:06.314 |
|Go   | 00:00:00.088 | 00:00:08.902 |
|F#   | 00:00:00.480 | 00:00:46.044 |
|C#   | 00:00:00.380 |              |
|rust | 00:00:00.050 | 00:00:05.800 |
