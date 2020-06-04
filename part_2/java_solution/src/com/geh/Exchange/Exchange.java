package com.geh.Exchange;

import java.util.Map;
import java.util.HashMap;

public class Exchange {

    public Trade[] execute(Order order)
    {
        var orderBook = orderBooks.get(order.getInstrument());

        if (orderBook == null) {
            orderBook = new OrderBook();
            orderBooks.put(order.getInstrument(), orderBook);
        }

        return orderBook.execute(order);
    }

    private Map<String, OrderBook> orderBooks = new HashMap<>();

}
