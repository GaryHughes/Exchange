package main

import (
	"fmt"
	"sort"
	"strings"
)

type OrderBook struct {
	Buys       []Order
	Sells      []Order
	Instrument string
	nextgen    uint64
}

func (ob *OrderBook) String() string {
	var sb strings.Builder
	n := len(ob.Buys)
	if n < len(ob.Sells) {
		n = len(ob.Sells)
	}
	sb.WriteString(strings.Repeat("-", 43))
	sb.WriteString(fmt.Sprintf("\n| %-39s |\n", ob.Instrument))
	sb.WriteString(strings.Repeat("-", 43))
	sb.WriteString(fmt.Sprintf("\n%6s %6s %6s | %6s %6s %6s\n",
		"BUYS", "Qty", "Px",
		"Px", "Qty", "SELLS"))
	for i := 0; i < n; i++ {
		var b, s string
		if i < len(ob.Buys) {
			b = fmt.Sprintf("%6d %6.2f", ob.Buys[i].Remaining, ob.Buys[i].Price)
		}
		if i < len(ob.Sells) {
			s = fmt.Sprintf("%6.2f %6d", ob.Sells[i].Price, ob.Sells[i].Remaining)
		}
		row := fmt.Sprintf("%20s | %-20s\n", b, s)
		sb.WriteString(row)
	}
	return sb.String()
}

func (ob *OrderBook) Insert(o Order) {
	o.gen = ob.nextgen
	ob.nextgen++
	if o.IsBuy {
		ob.Buys = append(ob.Buys, o)
		sort.Slice(ob.Buys, func(i, j int) bool {
			return ob.Buys[i].Price > ob.Buys[j].Price
		})
	} else {
		ob.Sells = append(ob.Sells, o)
		sort.Slice(ob.Sells, func(i, j int) bool {
			return ob.Sells[i].Price < ob.Sells[j].Price
		})
	}
}

func (ob *OrderBook) Match() ([]Trade, error) {
	ts := []Trade{}
	for len(ob.Buys) > 0 && len(ob.Sells) > 0 {
		buy := &ob.Buys[0]
		sell := &ob.Sells[0]
		if buy.Price < sell.Price {
			break
		}
		t := Trade{
			Buyer:      buy.Participant,
			Seller:     sell.Participant,
			Instrument: buy.Instrument,
		}

		if buy.gen < sell.gen {
			t.Price = buy.Price
		} else {
			t.Price = sell.Price
		}
		if buy.Remaining > sell.Remaining {
			t.Quantity = sell.Remaining
		} else {
			t.Quantity = buy.Remaining
		}

		ts = append(ts, t)
		err := buy.Fill(t.Quantity)
		if err != nil {
			return ts, err
		}
		if buy.Remaining == 0 {
			ob.Buys = ob.Buys[1:]
		}
		err = sell.Fill(t.Quantity)
		if err != nil {
			return ts, err
		}
		if sell.Remaining == 0 {
			ob.Sells = ob.Sells[1:]
		}
	}
	return ts, nil
}
