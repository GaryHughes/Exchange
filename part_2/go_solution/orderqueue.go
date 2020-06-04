package main

type OrderQueue []*Order

func (q OrderQueue) Len() int            { return len(q) }
func (q OrderQueue) Swap(i, j int)       { q[i], q[j] = q[j], q[i] }
func (q OrderQueue) Peek() *Order        { return q[0] }
func (q *OrderQueue) Push(x interface{}) { *q = append(*q, x.(*Order)) }
func (q *OrderQueue) Pop() interface{} {
	old := *q
	n := len(old)
	x := old[n-1]
	*q = old[0 : n-1]
	return x
}

type BuyQueue struct{ OrderQueue }
type SellQueue struct{ OrderQueue }

func (q BuyQueue) Less(i, j int) bool {
	ii := q.OrderQueue[i]
	jj := q.OrderQueue[j]
	// TODO: shouldn't compare floats for equality, this would
	// break if there was any maths done on them...
	if ii.Price == jj.Price {
		return ii.gen < jj.gen
	}
	return ii.Price > jj.Price
}
func (q SellQueue) Less(i, j int) bool {
	ii := q.OrderQueue[i]
	jj := q.OrderQueue[j]
	if ii.Price == jj.Price {
		return ii.gen < jj.gen
	}
	return ii.Price < jj.Price
}
