![Part 1 - C++](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20C++/badge.svg) ![Part 2 - C++](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20C++/badge.svg) ![Part 1 - C#](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20C%23/badge.svg) ![Part 2 - C#](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20C%23/badge.svg) ![Part 1 - F#](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20F%23/badge.svg) ![Part 2 - F#](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20F%23/badge.svg) ![Part 1 - Go](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20Go/badge.svg) ![Part 2 - Go](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20Go/badge.svg) ![Part 1 - Rust](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20Rust/badge.svg) ![Part 2 - Rust](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20Rust/badge.svg) ![Part 1 - Swift](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20Swift/badge.svg) ![Part 2 - Swift](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20Swift/badge.svg) ![Part 1 - Java](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20Java/badge.svg) ![Part 2 - Java](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20Java/badge.svg) ![Part 1 - Python](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20Python/badge.svg) ![Part 2 - Python](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20Python/badge.svg) ![Part 1 - C](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20C/badge.svg) ![Part 2 - C](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20C/badge.svg) ![Part 1 - TypeScript](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20TypeScript/badge.svg) ![Part 2 - TypeScript](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20TypeScript/badge.svg) ![Benchmark](https://github.com/GaryHughes/Exchange/workflows/Benchmark/badge.svg) 

# What is this?

Exchange is a playground to experiment with and compare programming languages by solving a simple problem in your language of choice and then seeing how fast you can make it run.

The problem is broken into two parts, for [Part 1](https://github.com/GaryHughes/Exchange/tree/master/Part%201) the goal is to solve the basic problem using idiomatic, expressive, readable code without too much though given to performance. For [Part 2](https://github.com/GaryHughes/Exchange/tree/master/Part%202) all bets are off and the goal is to make it run as fast as possible.

# Benchmarks

The solutions are benchmarked by taking the build artifacts for each language and running them 5 times over the supplied input files. The median runtime is recorded. All solutions are run on the same container instance sequentially, the results will vary depending on what resources the particular machine has at run time and will typcially be noticably slower than the results you will see on modern desktop hardware.

Some solutions are excluded from the benchmarks entirely because they take too long to run even once. Others are exluded from the benchmark with the larger file because they take too long for that particular input.

## Part 1 Performance


cannot access local variable 'i' where it is not associated with a value
cannot access local variable 'i' where it is not associated with a value
cannot access local variable 'i' where it is not associated with a value
||100K orders|trades|
-|:-:|:-:|
|F#|0:00:00.224637|4|
|C#|0:00:00.560456|3368|
|Go|0:00:05.511286|3368|
|Swift|0:00:05.698202|3368|
|C|0:00:09.217424|3368|
|C++|0:00:18.645199|3368|
|Python|0:01:24.725058|3368|


## Part 2 Performance

The slower implementations are excluded from the larger benchmark just so it runs in a reasonable time.

cannot access local variable 'i' where it is not associated with a value
cannot access local variable 'i' where it is not associated with a value
cannot access local variable 'i' where it is not associated with a value
cannot access local variable 'i' where it is not associated with a value
cannot access local variable 'i' where it is not associated with a value
cannot access local variable 'i' where it is not associated with a value
||100K orders|trades|
-|:-:|:-:|
|C|0:00:00.022761|3368|
|C++|0:00:00.038005|3368|
|Go|0:00:00.045679|3368|
|C#|0:00:00.208490|4|
|F#|0:00:00.212190|4|
|Python|0:00:00.292944|3368|
|Swift|0:00:00.530287|3368|


||10M orders|trades|
-|:-:|:-:|
|C#|0:00:00.196818|4|
|F#|0:00:00.200142|4|
|C|0:00:02.055578|360131|
|C++|0:00:03.387506|360131|
|Go|0:00:05.127518|360131|


