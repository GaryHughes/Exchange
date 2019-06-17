package main

import (
	"fmt"
	"math"
	"testing"
	"testing/quick"

	"gotest.tools/assert"
)

func TestValidOrder(t *testing.T) {
	f := func(party, inst string, qty int64, px uint16) bool {
		o, err := scanOrder(party, inst, qty, px)
		assert.NilError(t, err)

		buy := true
		if qty < 0 {
			buy = false
			qty = -qty
		}

		expectedPrice := math.Abs(float64(px) / 100)
		pxDiff := math.Abs(float64(o.Price) - expectedPrice)
		assert.Equal(t, o.Participant, party)
		assert.Equal(t, o.Instrument, inst)
		assert.Equal(t, o.Quantity, Quantity(qty))
		assert.Equal(t, o.Remaining, Quantity(qty))
		assert.Equal(t, o.IsBuy, buy)
		assert.Assert(t, pxDiff < 0.009)
		return true
	}
	if err := quick.Check(f, nil); err != nil {
		t.Error(err)
	}
}

func TestOrderFill(t *testing.T) {
	o := &Order{
		Participant: "A",
		Instrument:  "COWBEL",
		Quantity:    100,
		Remaining:   50,
		Price:       1,
		IsBuy:       true,
		gen:         23,
	}
	err := o.Fill(30)
	assert.NilError(t, err)
	assert.Equal(t, o.Remaining, Quantity(20))

	err = o.Fill(20)
	assert.NilError(t, err)
	assert.Equal(t, o.Remaining, Quantity(0))

	err = o.Fill(1)
	assert.ErrorContains(t, err, "insufficient quantity remaining")

	// other order properties should not have changed
	assert.Equal(t, o.Participant, "A")
	assert.Equal(t, o.Instrument, "COWBEL")
	assert.Equal(t, o.Quantity, Quantity(100))
	assert.Equal(t, o.Price, Price(1))
	assert.Equal(t, o.IsBuy, true)
	assert.Equal(t, o.gen, uint64(23))
}

func BenchmarkScanOrder(b *testing.B) {
	row := []string{"A", "COWBEL", "1000", "2"}
	for n := 0; n < b.N; n++ {
		_, _ = ScanOrder(row)
	}
}

func scanOrder(party, inst string, qty int64, px uint16) (*Order, error) {
	sqty := fmt.Sprintf("%d", qty)
	dollars, cents := px/100, px%100
	spx := fmt.Sprintf("%d.%02d", dollars, cents)
	o, err := ScanOrder([]string{party, inst, sqty, spx})
	if err != nil {
		return nil, err
	}
	return o, nil
}
