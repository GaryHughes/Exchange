package com.geh.Exchange;

import java.math.BigDecimal;

public class Order {

    private String participant;
    private String instrument;
    private long quantity;
    private long remainingQuantity;
    private BigDecimal price;
    private int generation;

    public Order(String line) throws Exception {
        var components = line.split(":");
        if (components.length != 4) {
            throw new Exception("Invalid line '" + line + "' expected 4 components and found " + String.valueOf(components.length));
        }

        this.participant = components[0];
        this.instrument = components[1];
        this.quantity = Long.parseLong(components[2]);
        this.remainingQuantity = Math.abs(quantity);
        this.price = new BigDecimal(components[3]);
        this.generation = nextGeneration++;
    }

    public String getParticipant() { return participant; }
    public String getInstrument() { return instrument; }
    public long getQuantity() { return quantity; }
    public long getRemainingQuantity() { return remainingQuantity; }
    public BigDecimal getPrice() { return price; }
    public int getGeneration() { return generation; }

    public void fill(long quantity)
    {
        remainingQuantity -= quantity;
    }

    private static int nextGeneration = 1;

}

