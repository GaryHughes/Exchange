package main

import (
	"encoding/csv"
	"fmt"
	"io"
	"os"
)

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
		trades, err := exchange.Execute(o)
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
