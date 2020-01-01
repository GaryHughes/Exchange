package com.geh.Exchange;

import java.math.BigDecimal;

public class Trade {

    private String buyer;
    private String seller;
    private String instrument;
    private long quantity;
    private BigDecimal price;

    public Trade(String buyer, String seller, String instrument, long quantity, BigDecimal price)
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
    public BigDecimal getPrice() { return price; }

}
