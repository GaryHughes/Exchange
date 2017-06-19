import Foundation

class Exchange
{
    var orderBooks : [String : OrderBook] = [:]

    func insert(order: Order) -> [Trade]
    {
        var orderBook = orderBooks[order.instrument]

        if orderBook == nil
        {
            orderBook = OrderBook()
            orderBooks[order.instrument] = orderBook
        }

        let trades = orderBook!.insert(order: order)
        return trades
    }
}

