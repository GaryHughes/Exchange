package main

import (
	"strconv"
	"strings"
)

const tenPow10 = 10000000000 // can write 10_000_000_000 in go 1.13

type Fixed10 struct {
	v int
}

func ParseFixed10(s string) (Fixed10, error) {
	point := strings.IndexRune(s, '.')
	if point == -1 {
		a, err := strconv.Atoi(s)
		if err != nil {
			return Fixed10{}, err
		}
		return Fixed10{v: a * tenPow10}, nil
	}

	a, err := strconv.Atoi(s[0:point])
	if err != nil {
		return Fixed10{}, err
	}

	b := 0
	for i := point + 1; i < point+11; i++ {
		b *= 10
		if i < len(s) {
			b += int(s[i] - '0')
		}
	}

	if s[0] == '-' {
		b = -b
	}

	return Fixed10{v: (a * tenPow10) + b}, nil
}

func (f Fixed10) String() string {
	v := f.v
	var sb strings.Builder

	if v < 0 {
		sb.WriteRune('-')
		v = 0 - v
	}

	a, b := v/tenPow10, v%tenPow10
	sb.WriteString(strconv.Itoa(a))

	n := 5
	if n == 0 {
		return sb.String()
	}

	sb.WriteRune('.')

	p := tenPow10
	for i := 0; i < n; i++ {
		p /= 10
		d := (b / p) % 10
		sb.WriteRune(rune(int('0') + d))
	}

	return sb.String()
}

func (f Fixed10) StringPrec(n int) string {
	v := f.v
	var sb strings.Builder

	if v < 0 {
		sb.WriteRune('-')
		v = 0 - v
	}

	a, b := v/tenPow10, v%tenPow10
	sb.WriteString(strconv.Itoa(a))

	if n == 0 {
		return sb.String()
	}

	sb.WriteRune('.')

	p := tenPow10
	for i := 0; i < n; i++ {
		p /= 10
		d := (b / p) % 10
		sb.WriteRune(rune(int('0') + d))
	}

	return sb.String()
}

func (f Fixed10) IsLess(b Fixed10) bool {
	return f.v < b.v
}
