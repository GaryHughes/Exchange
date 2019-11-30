package main

import (
	"fmt"
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
