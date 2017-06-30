
class OrderBook
{
    var bids : [Order]
    var asks : [Order]

    public init()
    {
        bids = []
        asks = []
    }

    public func insert(order: Order) -> [Trade]
    {
        let trades = match(order: order)

        if order.quantity > 0
        {
            insert(order, inSide: &bids, where: { $0.price < order.price })
        }

        if order.quantity < 0
        {
            insert(order, inSide: &asks, where: { $0.price > order.price })
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
                    let trade = execute(order, with: ask)

                    ask.quantity += trade.quantity
                    order.quantity -= trade.quantity

                    trades.append(trade)
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
                    let trade = execute(order, with: bid)

                    bid.quantity -= trade.quantity
                    order.quantity += trade.quantity

                    trades.append(trade)
                }
            }

            bids = bids.filter() { $0.quantity > 0 }
        }

        return trades
    }

    private func insert(_ order: Order, inSide side: inout [Order], where condition: (Order) -> Bool)
    {
        if let index = side.index(where: condition)
        {
            side.insert(order, at: index)
        }
        else
        {
            side.append(order)
        }
    }

    private func execute(_ order: Order, with other: Order) -> Trade
    {
        let trade = Trade()

        trade.instrument = order.instrument
        trade.quantity = min(abs(order.quantity), abs(other.quantity))
        trade.price = other.price

        if order.quantity > 0
        {
            trade.buyer = order.participant
            trade.seller = other.participant
        }
        else
        {
            trade.buyer = other.participant
            trade.seller = order.participant
        }

        return trade
    }
}
