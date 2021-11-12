using System;
using System.Collections.Generic;

namespace Exchange;

public class Exchange
{
    public Trade[] Execute(Order order)
    {
        if (!OrderBooks.TryGetValue(order.Instrument, out var orderBook)) {
            orderBook = new OrderBook();
            OrderBooks[order.Instrument] = orderBook;    
        }    
        return orderBook.Execute(order);
    }

    Dictionary<string, OrderBook> OrderBooks { get; } = new Dictionary<string, OrderBook>();
}
