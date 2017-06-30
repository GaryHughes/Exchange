import Foundation

public class Exchange
{
    var orderBooks : [String : OrderBook] = [:]

    public init()
    {
    }

    public func insert(order: Order) -> [Trade]
    {
        var orderBook = orderBooks[order.instrument]

        if orderBook == nil
        {
            orderBook = OrderBook()
            orderBooks[order.instrument] = orderBook
        }

        return orderBook!.insert(order: order)
    }
}

