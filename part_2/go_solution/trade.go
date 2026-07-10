package main

import (
	"fmt"
	"strconv"
)

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

// AppendTo appends the trade's line representation (matching String, plus a
// trailing newline) to buf. Used on the hot output path to avoid fmt's
// reflection-based formatting, which is significant overhead at millions of
// trades - it type-switches on each interface{} argument at runtime instead
// of emitting a direct, type-specific conversion.
func (t Trade) AppendTo(buf []byte) []byte {
	buf = append(buf, t.Buyer...)
	buf = append(buf, ':')
	buf = append(buf, t.Seller...)
	buf = append(buf, ':')
	buf = append(buf, t.Instrument...)
	buf = append(buf, ':')
	buf = strconv.AppendInt(buf, int64(t.Quantity), 10)
	buf = append(buf, ':')
	buf = strconv.AppendFloat(buf, float64(t.Price), 'g', -1, 32)
	buf = append(buf, '\n')
	return buf
}
