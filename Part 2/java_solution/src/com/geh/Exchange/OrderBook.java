package com.geh.Exchange;

import java.util.PriorityQueue;
import java.util.LinkedList;

public class OrderBook {

    public Trade[] execute(Order order)
    {
        if (order.getQuantity() > 0) {
            buyOrders.add(order);
        }
        else {
            sellOrders.add(order);
        }

        var trades = new LinkedList<Trade>();

        while (!buyOrders.isEmpty() && !sellOrders.isEmpty()) {

            var buyOrder = buyOrders.peek();
            var sellOrder = sellOrders.peek();

            if (buyOrder.getPrice().compareTo(sellOrder.getPrice()) < 0) {
                break;
            }

            var quantity = Math.min(buyOrder.getRemainingQuantity(), sellOrder.getRemainingQuantity());
            var price = buyOrder.getGeneration() < sellOrder.getGeneration() ? buyOrder.getPrice() : sellOrder.getPrice();

            var trade = new Trade(buyOrder.getParticipant(), sellOrder.getParticipant(), buyOrder.getInstrument(), quantity, price);

            trades.add(trade);

            buyOrder.fill(quantity);
            sellOrder.fill(quantity);

            if (buyOrder.getRemainingQuantity() == 0) {
                buyOrders.remove(buyOrder);
            }

            if (sellOrder.getRemainingQuantity() == 0) {
                sellOrders.remove(sellOrder);
            }
        }

        return trades.toArray(Trade[]::new);
    }

    public PriorityQueue<Order> getBuyOrders() {
        return buyOrders;
    }

    public PriorityQueue<Order> getSellOrders() {
        return sellOrders;
    }

    private PriorityQueue<Order> buyOrders = new PriorityQueue<>(1000, (left, right) -> {
        var comparison = right.getPrice().compareTo(left.getPrice());
        if (comparison == 0) {
            return Integer.compare(left.getGeneration(), right.getGeneration());
        }
        return comparison;
    });

    private PriorityQueue<Order> sellOrders = new PriorityQueue<>(1000, (left, right) -> {
        var comparison = left.getPrice().compareTo(right.getPrice());
        if (comparison == 0) {
            return Integer.compare(left.getGeneration(), right.getGeneration());
        }
        return comparison;
    });

}
