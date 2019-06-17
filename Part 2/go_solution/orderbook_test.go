package main

import (
	"container/heap"
	"math/rand"
	"testing"
	"testing/quick"

	"gotest.tools/assert"
)

func TestOrderBookOrdering(t *testing.T) {
	book := NewOrderBook("")
	f := func(n uint8) bool {
		// create n orders
		for i := 0; i < int(n); i++ {
			px := Price(rand.Intn(100))
			buy := rand.Uint32()%2 == 0
			book.Insert(Order{
				Price: px,
				IsBuy: buy,
			})
		}
		// buys should be highest to lowest
		t.Log("buys", book.Buys.Len())
		if book.Buys.Len() > 0 {
			head := book.Buys.Peek()
			for book.Buys.Len() > 0 {
				next := heap.Pop(&book.Buys).(*Order)
				if head.Price < next.Price {
					t.Errorf("%v < %v", head, next)
				}
				head = next
			}
		}
		// sells should be lowest to highest
		t.Log("sells", book.Sells.Len())
		if book.Sells.Len() > 0 {
			head := book.Sells.Peek()
			for book.Sells.Len() > 0 {
				next := heap.Pop(&book.Sells).(*Order)
				if head.Price > next.Price {
					t.Errorf("%v > %v", head, next)
				}
				head = next
			}
		}
		return true
	}
	if err := quick.Check(f, nil); err != nil {
		t.Error(err)
	}
}

func TestFirstPriceEnteredSelected(t *testing.T) {
	book := NewOrderBook("")
	book.Insert(makeOrder("B", "EURUSD", -100, 1.11))
	book.Insert(makeOrder("F", "EURUSD", -50, 1.1))
	book.Insert(makeOrder("D", "EURUSD", 100, 1.11))

	trades, err := book.Match()
	assert.NilError(t, err)
	assert.Equal(t, len(trades), 2)
	assert.Equal(t, trades[0].Price, Price(1.1))
	assert.Equal(t, trades[0].Quantity, Quantity(50))
	assert.Equal(t, trades[1].Price, Price(1.11))
	assert.Equal(t, trades[1].Quantity, Quantity(50))
}

func makeOrder(party, inst string, qty int64, px float32) Order {
	buy := true
	if qty < 0 {
		buy = false
		qty = -qty
	}
	return Order{party, inst, Quantity(qty), Quantity(qty), Price(px), buy, 0}
}
