## Rust solution (Part 2)

Keeping in the spirit of the C++ solution, buy and sell queues are
`std::collections::BinaryHeap<Order>`. This is a max heap, and a pain
to use with different sorting criteria for the same type, so I made
Order so that a better priced order is greater than a later one, and
an earlier order is greater than a later one if the prices are
equal. BinaryHeap requires total ordering, which is not a property of
floats (NaN), but as there's no maths done on prices, we'll be right.

## Build tools

Install [rustup](https://www.rust-lang.org/tools/install)

## Building and running the Rust solution

```
% cargo build --release && cargo test && gzip -dc ../../orders-100K.txt.gz |  time ./target/release/exchange > ../rust-trades-100K.txt
```

## Profiling on macOS

http://carol-nichols.com/2017/04/20/rust-profiling-with-dtrace-on-osx/

Using dtrace and FlameGraph.

```
% gzip -dc ../../orders-10M.txt.gz|  sudo dtrace -c './target/release/exchange' -o out.stacks -n 'profile-997 /execname == "exchange"/ { @[ustack(100)] = count(); }' > /dev/null
% stackcollapse.pl out.stacks| flamegraph.pl > exchange-flames.svg
```
