package main

import (
	"encoding/csv"
	"io"
	"strconv"

	"github.com/pkg/errors"
)

type OrderReader struct {
	reader *csv.Reader
}

func NewOrderReader(in io.Reader) *OrderReader {
	r := csv.NewReader(in)
	r.FieldsPerRecord = 4
	r.Comma = ':'
	r.ReuseRecord = true

	return &OrderReader{reader: r}
}

func (s *OrderReader) Read() (*Order, error) {
	rec, err := s.reader.Read()
	if err == io.EOF {
		return nil, nil
	}
	if err != nil {
		return nil, err
	}
	o, err := buildOrder(rec)
	if err != nil {
		return nil, err
	}
	return o, nil
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
