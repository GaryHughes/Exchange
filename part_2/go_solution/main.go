package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	verbose := false
	exchange := NewExchange()
	reader := NewOrderReader(os.Stdin)

	// os.Stdout has no buffering of its own, so fmt.Println was issuing one
	// write() syscall per trade. Buffer stdout ourselves and flush once at
	// the end (the deferred Flush still runs during a panic unwind).
	out := bufio.NewWriterSize(os.Stdout, 64*1024)
	defer out.Flush()

	line := make([]byte, 0, 128)

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
		for i := range trades {
			line = trades[i].AppendTo(line[:0])
			if _, err := out.Write(line); err != nil {
				panic(err)
			}
		}
		if verbose {
			fmt.Println(exchange.book[o.Instrument])
		}
	}
}
