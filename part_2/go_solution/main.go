package main

import (
	"fmt"
	"os"
)

func main() {
	verbose := false
	exchange := NewExchange()
	reader := NewOrderReader(os.Stdin)
	for {
		o, err := reader.Read()
		if err != nil {
			panic(err)
		}
		if o == nil {
			break
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
