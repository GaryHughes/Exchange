package main

import (
	"fmt"
	"strconv"

	"github.com/pkg/errors"
)

// Gary would use a fixed point type for prices in a real implementation
type Quantity int64
type Price float32

type Order struct {
	Participant string
	Instrument  string
	Quantity    Quantity
	Remaining   Quantity
	Price       Price
	IsBuy       bool
	gen         uint64
}

func ScanOrder(r []string) (*Order, error) {
	type Token int
	const (
		participantField Token = iota
		instrumentField
		quantityField
		priceField
		requiredFields
	)
	qty, err := strconv.Atoi(r[quantityField])
	if err != nil {
		return nil, errors.Wrap(err, "parsing quantity")
	}
	px, err := strconv.ParseFloat(r[priceField], 64)
	if err != nil {
		return nil, errors.Wrap(err, "parsing price")
	}

	buy := qty > 0
	if !buy {
		qty = -qty
	}

	return &Order{
		IsBuy:       buy,
		Participant: r[participantField],
		Instrument:  r[instrumentField],
		Quantity:    Quantity(qty),
		Remaining:   Quantity(qty),
		Price:       Price(px),
	}, nil

}

func (o *Order) Fill(qty Quantity) error {
	if qty > o.Remaining {
		return fmt.Errorf("insufficient quantity remaining (%d)", o.Remaining)
	}
	o.Remaining -= qty
	return nil
}

func (o *Order) String() string {
	var bos string
	if o.IsBuy {
		bos = "B"
	} else {
		bos = "S"
	}
	return fmt.Sprintf("%s %s %v @ %v", bos, o.Instrument, o.Remaining, o.Price)
}
