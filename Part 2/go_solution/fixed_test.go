package main

import (
	// "math/rand"
	"testing"
	// "testing/quick"

	"gotest.tools/assert"
)

func TestParseDecimal(t *testing.T) {
	var v Fixed10
	var err error

	cases := []struct {
		inp    string
		places int
		exp    string
	}{
		{"654", 5, "654.00000"},
		{"654.0", 5, "654.00000"},
		{"0.321", 5, "0.32100"},
		{"0.1", 5, "0.10000"},
		{"0.01", 5, "0.01000"},
		{"0.010", 5, "0.01000"},
		{"654.321", 5, "654.32100"},
		{"654.001", 5, "654.00100"},
		{"654.000001", 5, "654.00000"},
		{"654.000001", 6, "654.000001"},
		{"0.0000000001", 10, "0.0000000001"},
	}

	for _, c := range cases {
		v, err = ParseFixed10(c.inp)
		t.Log(v.v, v.StringPrec(c.places))
		assert.NilError(t, err)
		assert.Equal(t, v.StringPrec(c.places), c.exp)
	}

	for _, c := range cases {
		v, err = ParseFixed10("-" + c.inp)
		t.Log("-"+c.inp, v.v, v.StringPrec(c.places))
		assert.NilError(t, err)
		assert.Equal(t, v.StringPrec(c.places), "-"+c.exp)
	}
}

func TestParseZero(t *testing.T) {
	var v Fixed10
	var err error

	cases := []struct {
		inp    string
		places int
		exp    string
	}{
		{"0.0", 0, "0"},
		{"0.0", 1, "0.0"},
		{"0.0", 10, "0.0000000000"},
		{"0.0000000000", 10, "0.0000000000"},
	}

	for _, c := range cases {
		v, err = ParseFixed10(c.inp)
		t.Log(v.v, v.StringPrec(c.places))
		assert.NilError(t, err)
		assert.Equal(t, v.StringPrec(c.places), c.exp)
	}

	// negative zero is printed as positive zero
	for _, c := range cases {
		v, err = ParseFixed10("-" + c.inp)
		t.Log("-"+c.inp, v.v, v.StringPrec(c.places))
		assert.NilError(t, err)
		assert.Equal(t, v.StringPrec(c.places), c.exp)
	}
}
