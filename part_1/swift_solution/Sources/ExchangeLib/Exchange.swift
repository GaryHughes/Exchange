import Foundation
import Dispatch

public class Exchange
{
    var orderBooks : [String : OrderBook] = [:]
    var dispatchQueues : [String : DispatchQueue] = [:]
    var tradeQueue = DispatchQueue(label: "Trades")
    public var trades : [Trade] = []

    public init()
    {
    }

    public func insert(order: Order)
    {
        var orderBook = orderBooks[order.instrument]

        if orderBook == nil
        {
            orderBook = OrderBook()
            orderBooks[order.instrument] = orderBook
            dispatchQueues[order.instrument] = DispatchQueue(label: order.instrument)
        }

        if let dispatchQueue = dispatchQueues[order.instrument]
        {
            dispatchQueue.async
            {
                let trades = orderBook!.insert(order: order)

                self.tradeQueue.async
                {
                    self.trades += trades
                }
            }
        }
    }

    public func waitForTrades()
    {
        for dispatchQueue in dispatchQueues.values
        {
            dispatchQueue.sync {}
        }

        tradeQueue.sync {}
    }
}

