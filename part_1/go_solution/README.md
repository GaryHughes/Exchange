## Go solution (Part 1)

Trying to match the C++ solution in style, buy and sell queues are Go
arrays, sorted after each insert by price then time priority.

## Build tools

Install [go](https://golang.org)

## Building and running the Go solution

```
% go build && gzip -dc ../../orders-100K.txt.gz | time ./Exchange > trades.txt
```
