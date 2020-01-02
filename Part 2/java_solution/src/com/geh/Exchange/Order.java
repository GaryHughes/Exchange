package com.geh.Exchange;

import java.math.BigDecimal;
import java.util.LinkedList;

public class Order {

    private String participant;
    private String instrument;
    private long quantity;
    private long remainingQuantity;
    private double price;
    private int generation;

    private enum Token {
        Participant,
        Instrument,
        Quantity,
        Price
    };

    public void Initialise(String line) throws Exception {

        var token = Token.Participant;
        int start = 0;
        for (int i = start; (i = line.indexOf(":", start)) != -1; ) {
            switch (token) {
                case Participant:
                    this.participant = line.substring(start, i);
                    token = Token.Instrument;
                    break;
                case Instrument:
                    this.instrument = line.substring(start, i);
                    token = Token.Quantity;
                    break;
                case Quantity:
                    this.quantity = Long.parseLong(line.subSequence(start, i), 0, i - start, 10);
                    this.remainingQuantity = Math.abs(quantity);
                    token = Token.Price;
                    break;
                default:
                    break;
            }

            start = i + 1;
        }

        if (token != Token.Price) {
            throw new Exception("cannot parse: " + line);
        }

        this.price = Double.parseDouble(line.substring(start));
        this.generation = nextGeneration++;
    }

    public String getParticipant() { return participant; }
    public String getInstrument() { return instrument; }
    public long getQuantity() { return quantity; }
    public long getRemainingQuantity() { return remainingQuantity; }
    public double getPrice() { return price; }
    public int getGeneration() { return generation; }

    public void fill(long quantity)
    {
        remainingQuantity -= quantity;
    }

    private static int nextGeneration = 1;

    private static LinkedList<Order> retiredOrders = new LinkedList<>();

    public static Order get(String line) throws Exception
    {
        Order order;
        if (retiredOrders.isEmpty()) {
            order = new Order();
        }
        else {
            order = retiredOrders.pop();
        }
        order.Initialise(line);
        return order;
    }

    public static void retire(Order order)
    {
        retiredOrders.add(order);
    }

}

