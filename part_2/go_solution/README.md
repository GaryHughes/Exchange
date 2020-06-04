## Go solution (Part 2)

Use a priority queue for buy and sell queues, to avoid sorting.

## Build tools

Install [go](https://golang.org)

## Building and running the Go solution

```
% go build && gzip -dc ../../orders-100K.txt.gz | time ./Exchange > trades.txt
```
