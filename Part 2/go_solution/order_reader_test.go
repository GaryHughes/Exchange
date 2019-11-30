package main

import (
	"io"
	"strings"
	"testing"

	"gotest.tools/assert"
)

func BenchmarkBuildOrder(b *testing.B) {
	row := []string{"A", "COWBEL", "1000", "2"}
	for n := 0; n < b.N; n++ {
		_, _ = buildOrder(row)
	}
}

func TestReadOrder(t *testing.T) {
	var tests = []struct {
		in         string
		party      string
		instrument string
		qty        Quantity
		price      Price
		isBuy      bool
	}{
		{"A:COWBEL:1000:2", "A", "COWBEL", 1000, 2, true},
		{"A:COWBEL:-1000:2", "A", "COWBEL", 1000, 2, false},
	}
	for _, tc := range tests {
		t.Run(tc.in, func(t *testing.T) {
			sr := strings.NewReader(tc.in)
			reader := NewOrderReader(sr)
			o, err := reader.Read()
			assert.NilError(t, err)
			assert.Assert(t, o != nil)
			assert.Equal(t, o.Participant, tc.party)
			assert.Equal(t, o.Instrument, tc.instrument)
			assert.Equal(t, o.Quantity, tc.qty)
			assert.Equal(t, o.Remaining, tc.qty)
			assert.Equal(t, o.Price, tc.price)
			assert.Equal(t, o.IsBuy, tc.isBuy)
		})
	}
}

// endless is an endlessly looping implementation of io.Reader
type endless struct {
	s []byte
	c int
}

func (e *endless) Read(b []byte) (n int, err error) {
	sl := len(e.s)
	bl := len(b)
	bc := 0
	for {
		n := copy(b[bc:], e.s[e.c:])
		bc += n
		e.c += n
		if bc == bl {
			break
		}
		if e.c == sl {
			e.c = 0
		}
	}

	return bl, nil
}

var _ io.Reader = &endless{}

func BenchmarkReadOrder(b *testing.B) {
	sr := &endless{s: []byte("A:COWBELL:1000:2\n")}
	reader := NewOrderReader(sr)
	b.ResetTimer()
	for n := 0; n < b.N; n++ {
		_, _ = reader.Read()
	}
}
