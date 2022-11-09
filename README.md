![Part 1 - C++](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20C++/badge.svg) ![Part 2 - C++](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20C++/badge.svg) ![Part 1 - C#](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20C%23/badge.svg) ![Part 2 - C#](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20C%23/badge.svg) ![Part 1 - F#](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20F%23/badge.svg) ![Part 2 - F#](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20F%23/badge.svg) ![Part 1 - Go](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20Go/badge.svg) ![Part 2 - Go](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20Go/badge.svg) ![Part 1 - Rust](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20Rust/badge.svg) ![Part 2 - Rust](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20Rust/badge.svg) ![Part 1 - Swift](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20Swift/badge.svg) ![Part 2 - Swift](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20Swift/badge.svg) ![Part 1 - Java](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20Java/badge.svg) ![Part 2 - Java](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20Java/badge.svg) ![Part 1 - Python](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20Python/badge.svg) ![Part 2 - Python](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20Python/badge.svg) ![Part 1 - Cython](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20Cython/badge.svg) ![Part 2 - Cython](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20Cython/badge.svg) ![Part 1 - C](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20C/badge.svg) ![Part 2 - C](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20C/badge.svg) ![Part 1 - TypeScript](https://github.com/GaryHughes/Exchange/workflows/Part%201%20-%20TypeScript/badge.svg) ![Part 2 - TypeScript](https://github.com/GaryHughes/Exchange/workflows/Part%202%20-%20TypeScript/badge.svg) ![Benchmark](https://github.com/GaryHughes/Exchange/workflows/Benchmark/badge.svg) 

# What is this?

Exchange is a playground to experiment with and compare programming languages by solving a simple problem in your language of choice and then seeing how fast you can make it run.

The problem is broken into two parts, for [Part 1](https://github.com/GaryHughes/Exchange/tree/master/Part%201) the goal is to solve the basic problem using idiomatic, expressive, readable code without too much though given to performance. For [Part 2](https://github.com/GaryHughes/Exchange/tree/master/Part%202) all bets are off and the goal is to make it run as fast as possible.

# Benchmarks

The solutions are benchmarked by taking the build artifacts for each language and running them 5 times over the supplied input files. The median runtime is recorded. All solutions are run on the same container instance sequentially, the results will vary depending on what resources the particular machine has at run time and will typcially be noticably slower than the results you will see on modern desktop hardware.

Some solutions are excluded from the benchmarks entirely because they take too long to run even once. Others are exluded from the benchmark with the larger file because they take too long for that particular input.

## Part 1 Performance


||100K orders|trades|
-|:-:|:-:|
|C#|0:00:00.675443|3368|
|Java|0:00:00.974463|3368|
|Rust|0:00:01.627709|3368|
|TypeScript|0:00:01.640852|3368|
|F#|0:00:07.718163|3368|
|Swift|0:00:10.833441|3368|
|C|0:00:14.616933|3368|
|C++|0:00:26.898889|3368|
|Go|0:00:50.956829|3368|
|Cython|0:02:52.393152|3368|
|Python|0:03:00.576237|3368|


## Part 2 Performance

The slower implementations are excluded from the larger benchmark just so it runs in a reasonable time.

||100K orders|trades|
-|:-:|:-:|
|C|0:00:00.043022|3368|
|Rust|0:00:00.053160|3368|
|C++|0:00:00.059976|3368|
|Go|0:00:00.086322|3368|
|Cython|0:00:00.122440|3368|
|C#|0:00:00.183962|3368|
|Java|0:00:00.376129|3368|
|F#|0:00:00.402953|3368|
|Python|0:00:00.467778|3368|
|TypeScript|0:00:00.961107|3368|
|Swift|0:00:02.057409|3368|


||10M orders|trades|
-|:-:|:-:|
|C|0:00:03.735741|360131|
|Rust|0:00:05.537702|360131|
|C++|0:00:06.785845|360131|
|Go|0:00:09.687977|360131|
|Cython|0:00:10.442227|360131|
|Java|0:00:12.978258|360131|
|C#|0:00:14.478067|360131|
|TypeScript|0:00:28.731660|360131|
|F#|0:00:31.773959|360131|


