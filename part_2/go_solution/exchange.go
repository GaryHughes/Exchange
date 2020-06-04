package main

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
	ob := NewOrderBook(ins)
	ex.book[ins] = ob
	return ob
}

func (ex *Exchange) Execute(o *Order) ([]Trade, error) {
	ob := ex.Book(o.Instrument)
	ob.Insert(o)
	return ob.Match()
}
