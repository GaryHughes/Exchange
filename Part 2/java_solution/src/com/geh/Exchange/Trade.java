package com.geh.Exchange;

import java.math.BigDecimal;

public class Trade {

    private String buyer;
    private String seller;
    private String instrument;
    private long quantity;
    private double price;

    public Trade(String buyer, String seller, String instrument, long quantity, double price)
    {
        this.buyer = buyer;
        this.seller = seller;
        this.instrument = instrument;
        this.quantity = quantity;
        this.price = price;
    }

    public String getBuyer() { return buyer; }
    public String getSeller() { return seller; }
    public String getInstrument() { return instrument; }
    public long getQuantity() { return quantity; }
    public double getPrice() { return price; }

}
