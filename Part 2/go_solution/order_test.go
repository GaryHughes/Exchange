package main

import (
	"testing"

	"gotest.tools/assert"
)

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
