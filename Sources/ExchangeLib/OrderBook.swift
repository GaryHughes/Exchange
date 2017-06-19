
class OrderBook
{
    var bids : [Order]
    var asks : [Order]

    init()
    {
        bids = []
        asks = []
    }

    func insert(order: Order) -> [Trade]
    {
        let trades = match(order: order)

        if order.quantity > 0
        {
            if let index = bids.index(where: { $0.price < order.price })
            {
                bids.insert(order, at: index)
            }
            else
            {
                bids.append(order)
            }
        }

        if order.quantity < 0
        {
            if let index = asks.index(where: { $0.price > order.price })
            {
                asks.insert(order, at: index)
            }
            else
            {
                asks.append(order)    
            }
        }

        return trades
    }

    private func match(order: Order) -> [Trade]
    {
        var trades : [Trade] = []

        if order.quantity > 0
        {
            for ask in asks
            {
                if order.quantity <= 0
                {
                    break
                }

                if ask.price <= order.price
                {
                    let trade = Trade()
                    trade.buyer = order.trader
                    trade.seller = ask.trader
                    trade.instrument = order.instrument
                    trade.quantity = min(abs(ask.quantity), abs(order.quantity))
                    trade.price = ask.price
                    trades.append(trade)

                    ask.quantity += trade.quantity
                    order.quantity -= trade.quantity
                }
            }

            asks = asks.filter() { $0.quantity < 0 }
        }

        if order.quantity < 0
        {
            for bid in bids
            {
                if order.quantity >= 0
                {
                    break
                }

                if bid.price >= order.price
                {
                    let trade = Trade()
                    trade.buyer = bid.trader
                    trade.seller = order.trader
                    trade.instrument = order.instrument
                    trade.quantity = min(abs(bid.quantity), abs(order.quantity))
                    trade.price = bid.price
                    trades.append(trade)

                    bid.quantity -= trade.quantity
                    order.quantity += trade.quantity
                }
            }

            bids = bids.filter() { $0.quantity > 0 }
        }

        return trades
    }
}
