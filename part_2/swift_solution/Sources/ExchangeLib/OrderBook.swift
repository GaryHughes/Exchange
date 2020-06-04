
class OrderBook
{
    var buyOrders: PriorityQueue<Order> = PriorityQueue<Order>(sort: { (lhs, rhs) -> Bool in
       
        assert(lhs.quantity >= 0)
        assert(lhs.quantity >= 0)
        
        if rhs.price == lhs.price {
            return lhs.generation < rhs.generation;
        }
        
        return rhs.price < lhs.price
    })
    
    var sellOrders: PriorityQueue<Order> = PriorityQueue<Order>(sort: { (lhs, rhs) -> Bool in
        
        assert(lhs.quantity < 0)
        assert(rhs.quantity < 0)
        
        if lhs.price == rhs.price {
            return lhs.generation < rhs.generation
        }
        
        return lhs.price < rhs.price
    })
    
    public func execute(order: Order) -> [Trade]
    {
        var trades : [Trade] = []

        if order.quantity > 0
        {
            buyOrders.enqueue(order)
        }

        if order.quantity < 0
        {
            sellOrders.enqueue(order)
        }
        
        while !buyOrders.isEmpty && !sellOrders.isEmpty
        {
            let buy = buyOrders.peek()!
            let sell = sellOrders.peek()!
        
            if buy.price < sell.price {
                break
            }
            
            let quantity = min(buy.remainingQuantity, sell.remainingQuantity)
            let price = buy.generation < sell.generation ? buy.price : sell.price

            let trade = Trade(buyer: buy.participant,
                              seller: sell.participant,
                              instrument: buy.instrument,
                              quantity: quantity,
                              price: price)
                        
            trades.append(trade)

            buy.fill(amount:quantity)
            sell.fill(amount:quantity)

            if buy.remainingQuantity == 0 {
                let _ = buyOrders.dequeue()
            }

            if sell.remainingQuantity == 0 {
                let _ = sellOrders.dequeue()
            }
        }
        
        return trades
    }
}
