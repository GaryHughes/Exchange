package main

import (
	"bufio"
	"io"
	"strconv"
	"strings"

	"github.com/pkg/errors"
)

type OrderReader struct {
	reader *bufio.Reader
	rec    []string
}

func NewOrderReader(in io.Reader) *OrderReader {
	r := bufio.NewReader(in)
	rec := make([]string, 4)
	return &OrderReader{reader: r, rec: rec}
}

func (s *OrderReader) Read() (*Order, error) {
	sline, err := s.reader.ReadSlice('\n')
	if err == io.EOF {
		return nil, nil
	}
	if err != nil {
		return nil, err
	}
	line := string(sline)
	d0 := strings.IndexRune(line, ':')
	d1 := d0 + 1 + strings.IndexRune(line[d0+1:], ':')
	d2 := d1 + 1 + strings.IndexRune(line[d1+1:], ':')
	s.rec[0] = line[0:d0]
	s.rec[1] = line[d0+1 : d1]
	s.rec[2] = line[d1+1 : d2]
	s.rec[3] = line[d2+1 : len(line)-1]

	return buildOrder(s.rec)
}

func buildOrder(r []string) (*Order, error) {
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
