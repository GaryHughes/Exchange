package main

import (
	"container/heap"
	"fmt"
	"strings"
)

type OrderBook struct {
	Buys       BuyQueue
	Sells      SellQueue
	Instrument string
	nextgen    uint64
}

func NewOrderBook(inst string) *OrderBook {
	ob := OrderBook{Instrument: inst}
	ob.Buys = BuyQueue{make(OrderQueue, 0, 100)}
	ob.Sells = SellQueue{make(OrderQueue, 0, 100)}
	heap.Init(&ob.Buys)
	heap.Init(&ob.Sells)
	return &ob
}

func (ob *OrderBook) String() string {
	buys := BuyQueue{append(OrderQueue(nil), ob.Buys.OrderQueue...)}
	sells := SellQueue{append(OrderQueue(nil), ob.Sells.OrderQueue...)}

	var sb strings.Builder
	sb.WriteString(strings.Repeat("-", 49))
	sb.WriteString(fmt.Sprintf("\n| %-45s |\n", ob.Instrument))
	sb.WriteString(strings.Repeat("-", 49))
	sb.WriteString(fmt.Sprintf("\n%6s %6s %9s | %9s %6s %6s\n",
		"BUYS", "Qty", "Px",
		"Px", "Qty", "SELLS"))
	rows := 0
	for rows < 10 && buys.Len() > 0 || sells.Len() > 0 {
		rows++
		var b, s string
		if buys.Len() > 0 {
			top := heap.Pop(&buys).(*Order)
			b = fmt.Sprintf("%6d %9.5f", top.Remaining, top.Price)
		}
		if sells.Len() > 0 {
			top := heap.Pop(&sells).(*Order)
			s = fmt.Sprintf("%9.5f %6d", top.Price, top.Remaining)
		}
		row := fmt.Sprintf("%23s | %-23s\n", b, s)
		sb.WriteString(row)
	}
	var b, s string
	if buys.Len() > 0 {
		b = fmt.Sprintf("%v more", buys.Len())
	}
	if sells.Len() > 0 {
		s = fmt.Sprintf("%v more", sells.Len())
	}
	if b != "" || s != "" {
		sb.WriteString(fmt.Sprintf("%23s | %-23s\n", b, s))
	}
	return sb.String()
}

func (ob *OrderBook) Insert(o Order) {
	o.gen = ob.nextgen
	ob.nextgen++
	if o.IsBuy {
		heap.Push(&ob.Buys, &o)
	} else {
		heap.Push(&ob.Sells, &o)
	}
}

func (ob *OrderBook) Match() ([]Trade, error) {
	ts := []Trade{}
	for ob.Buys.Len() > 0 && ob.Sells.Len() > 0 {
		buy := ob.Buys.Peek()
		sell := ob.Sells.Peek()
		if buy.Price.IsLess(sell.Price.Fixed10) {
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
			heap.Pop(&ob.Buys)
		}
		err = sell.Fill(t.Quantity)
		if err != nil {
			return ts, err
		}
		if sell.Remaining == 0 {
			heap.Pop(&ob.Sells)
		}
	}
	return ts, nil
}
