package main

import (
	"testing"

	"gotest.tools/assert"
)

func TestMatchWithDifferentParticipants(t *testing.T) {
	exchange := NewExchange()

	trades, err := exchange.Execute(makeOrder("A", "AUDUSD", 100, 10))
	assert.NilError(t, err)
	assert.Equal(t, len(trades), 0)

	trades, err = exchange.Execute(makeOrder("B", "AUDUSD", -100, 10))
	assert.NilError(t, err)
	assert.Equal(t, len(trades), 1)

	trade := trades[0]
	assert.Equal(t, trade.Buyer, "A")
	assert.Equal(t, trade.Seller, "B")
	assert.Equal(t, trade.Quantity, Quantity(100))
	assert.Equal(t, trade.Price, Price(10))
}

func TestMatchWithSameParticipant(t *testing.T) {
	exchange := NewExchange()

	trades, err := exchange.Execute(makeOrder("A", "AUDUSD", 100, 10))
	assert.NilError(t, err)
	assert.Equal(t, len(trades), 0)

	trades, err = exchange.Execute(makeOrder("A", "AUDUSD", -100, 10))
	assert.NilError(t, err)
	assert.Equal(t, len(trades), 1)

	trade := trades[0]
	assert.Equal(t, trade.Buyer, "A")
	assert.Equal(t, trade.Seller, "A")
	assert.Equal(t, trade.Quantity, Quantity(100))
	assert.Equal(t, trade.Price, Price(10))
}

func TestDifferentInstrumentsDoNotMatch(t *testing.T) {
	exchange := NewExchange()

	trades, err := exchange.Execute(makeOrder("A", "AUDUSD", 100, 10))
	assert.NilError(t, err)
	assert.Equal(t, len(trades), 0)

	trades, err = exchange.Execute(makeOrder("B", "EURGBP", -100, 10))
	assert.NilError(t, err)
	assert.Equal(t, len(trades), 0)
}

func TestPriceFromFirstOrderSelected(t *testing.T) {
	exchange := NewExchange()

	trades, err := exchange.Execute(makeOrder("A", "AUDUSD", 100, 10))
	assert.NilError(t, err)
	assert.Equal(t, len(trades), 0)

	trades, err = exchange.Execute(makeOrder("B", "AUDUSD", -100, 9))
	assert.NilError(t, err)
	assert.Equal(t, len(trades), 1)

	assert.Equal(t, trades[0].Price, Price(10))
}
