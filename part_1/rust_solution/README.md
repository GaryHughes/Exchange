## Rust solution (Part 1)

Keeping in the spirit of the C++ solution, buy and sell queues are
`Vec<Order>`, sorted after each insert by price then time
priority. The last element of the buy vector is the best (highest)
buy, and the last element of the sell vector is the best (lowest)
sell.

## Build tools

Install [rustup](https://www.rust-lang.org/tools/install)

## Building and running the Rust solution

```
% cargo build --release && cargo test && gzip -dc ../../orders-100K.txt.gz |  time ./target/release/exchange > ../rust-trades-100K.txt
```
