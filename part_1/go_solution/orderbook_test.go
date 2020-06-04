package main

import (
	"testing"

	"gotest.tools/assert"
)

func TestBuyOrdering(t *testing.T) {
	var book OrderBook
	book.Insert(makeOrder("A", "AUDUSD", 100, 10))
	book.Insert(makeOrder("A", "AUDUSD", 100, 7))
	book.Insert(makeOrder("A", "AUDUSD", 100, 11))
	book.Insert(makeOrder("A", "AUDUSD", 100, 9))
	assert.Equal(t, len(book.Buys), 4)
	assert.Equal(t, len(book.Sells), 0)
	assert.Equal(t, book.Buys[0].Price, Price(11))
	assert.Equal(t, book.Buys[1].Price, Price(10))
	assert.Equal(t, book.Buys[2].Price, Price(9))
	assert.Equal(t, book.Buys[3].Price, Price(7))
}

func TestSellOrdering(t *testing.T) {
	var book OrderBook
	book.Insert(makeOrder("A", "AUDUSD", -100, 10))
	book.Insert(makeOrder("A", "AUDUSD", -100, 7))
	book.Insert(makeOrder("A", "AUDUSD", -100, 11))
	book.Insert(makeOrder("A", "AUDUSD", -100, 9))
	assert.Equal(t, len(book.Buys), 0)
	assert.Equal(t, len(book.Sells), 4)
	assert.Equal(t, book.Sells[0].Price, Price(7))
	assert.Equal(t, book.Sells[1].Price, Price(9))
	assert.Equal(t, book.Sells[2].Price, Price(10))
	assert.Equal(t, book.Sells[3].Price, Price(11))
}

func TestFirstPriceEnteredSelected(t *testing.T) {
	var book OrderBook
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
