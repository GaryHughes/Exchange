
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
            trades += match(order, against: &asks, where: { $0.price <= order.price })
        }

        if order.quantity < 0
        {
            trades += match(order, against: &bids, where: { $0.price >= order.price })
        }

        return trades
    }

    private func insert(_ order: Order, inSide side: inout [Order], where condition: (Order) -> Bool)
    {
        if let index = side.firstIndex(where: condition)
        {
            side.insert(order, at: index)
        }
        else
        {
            side.append(order)
        }
    }

    private func match(_ order: Order, against side: inout [Order], where condition: (Order) -> Bool) -> [Trade]
    {
        var trades : [Trade] = []

        let sign = (order.quantity > 0 ? 1 : -1)

        for other_order in side.filter(condition)
        {
            if order.quantity == 0
            {
                break
            }

            let trade = execute(order, with: other_order)
            trades.append(trade)

            order.quantity -= sign * trade.quantity
            other_order.quantity += sign * trade.quantity
        }

        side = side.filter() { $0.quantity != 0 }

        return trades
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
