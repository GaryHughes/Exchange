package main

import "fmt"

type Trade struct {
	Buyer      string
	Seller     string
	Instrument string
	Quantity   Quantity
	Price      Price
}

func (t Trade) String() string {
	return fmt.Sprintf("%v:%v:%v:%v:%v",
		t.Buyer, t.Seller, t.Instrument,
		t.Quantity, t.Price)
}
