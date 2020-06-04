package main

import (
	"encoding/csv"
	"fmt"
	"io"
	"os"
)

type Exchange struct {
	book map[string]*OrderBook
}

func NewExchange() *Exchange {
	return &Exchange{book: make(map[string]*OrderBook)}
}

func (ex *Exchange) Book(ins string) *OrderBook {
	if ob, ok := ex.book[ins]; ok {
		return ob
	}
	ob := &OrderBook{Instrument: ins}
	ex.book[ins] = ob
	return ob
}

func (ex *Exchange) Execute(o Order) ([]Trade, error) {
	ob := ex.Book(o.Instrument)
	ob.Insert(o)
	return ob.Match()
}

func main() {
	verbose := false
	exchange := NewExchange()
	reader := csv.NewReader(os.Stdin)
	reader.FieldsPerRecord = 4
	reader.Comma = ':'
	reader.ReuseRecord = true
	for {
		rec, err := reader.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			panic(err)
		}
		o, err := ScanOrder(rec)
		if err != nil {
			panic(err)
		}

		if verbose {
			fmt.Println(o)
		}
		trades, err := exchange.Execute(*o)
		if err != nil {
			panic(err)
		}
		if len(trades) > 0 {
			for i := range trades {
				fmt.Println(trades[i])
			}
		}
		if verbose {
			fmt.Println(exchange.book[o.Instrument])
		}
	}
}
