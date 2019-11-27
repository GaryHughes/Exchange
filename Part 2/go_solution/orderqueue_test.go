package main

import (
	"container/heap"
	"math/rand"
	"testing"
)

// benchOrder is package local to defeat optimizations
var benchOrder *Order //nolint:gochecknoglobals

func BenchmarkQueuePush(b *testing.B) {
	q := BuyQueue{make(OrderQueue, 0, 0)}
	heap.Init(&q)
	o := Order{Price: Price(1.0)}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		o.gen++
		o.Price = Price(rand.Float32())
		heap.Push(&q, &o)
	}
}

func BenchmarkQueuePop(b *testing.B) {
	q := BuyQueue{make(OrderQueue, 0, 0)}
	heap.Init(&q)
	o := Order{Price: Price(1.0)}
	for i := 0; i < b.N; i++ {
		o.gen++
		o.Price = Price(rand.Float32())
		heap.Push(&q, &o)
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		benchOrder = heap.Pop(&q).(*Order)
	}
}

func BenchmarkQueuePeek(b *testing.B) {
	q := BuyQueue{make(OrderQueue, 0, 0)}
	heap.Init(&q)
	o := Order{Price: Price(1.0)}
	for i := 0; i < 100; i++ {
		o.gen++
		o.Price = Price(rand.Float32())
		heap.Push(&q, &o)
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		benchOrder = q.Peek()
	}
}
