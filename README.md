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
||100K orders|trades|
-|:-:|:-:|
|C#|0:00:00.554189|3368|
|F#|0:00:02.194715|3368|
|Go|0:00:05.536717|3368|
|Swift|0:00:05.590673|3368|
|C|0:00:09.234319|3368|
|Rust|0:00:11.339829|3368|
|C++|0:00:19.507282|3368|
|Python|0:01:16.135981|3368|


## Part 2 Performance

The slower implementations are excluded from the larger benchmark just so it runs in a reasonable time.

cannot access local variable 'i' where it is not associated with a value
cannot access local variable 'i' where it is not associated with a value
cannot access local variable 'i' where it is not associated with a value
cannot access local variable 'i' where it is not associated with a value
||100K orders|trades|
-|:-:|:-:|
|C|0:00:00.022754|3368|
|Rust|0:00:00.032016|3368|
|C++|0:00:00.037353|3368|
|Go|0:00:00.041358|3368|
|C#|0:00:00.179466|3368|
|Python|0:00:00.281752|3368|
|F#|0:00:00.345690|3368|
|Swift|0:00:00.524068|3368|


||10M orders|trades|
-|:-:|:-:|
|C|0:00:02.035755|360131|
|Rust|0:00:03.247212|360131|
|C++|0:00:03.357458|360131|
|Go|0:00:04.665971|360131|
|C#|0:00:07.976720|360131|
|F#|0:00:18.687489|360131|


