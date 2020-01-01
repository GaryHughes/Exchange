package com.geh.Exchange;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

import java.math.BigDecimal;

class OrderBookTest {

    @Test
    public void TestBuyOrdering() throws Exception
    {
    var book = new OrderBook();

        book.execute(new Order("A:AUDUSD:100:10"));
        book.execute(new Order("A:AUDUSD:100:7"));
        book.execute(new Order("A:AUDUSD:100:11"));
        book.execute(new Order("A:AUDUSD:100:9"));

        var buys = book.getBuyOrders();

        assertEquals(BigDecimal.valueOf(11), buys.poll().getPrice());
        assertEquals(BigDecimal.valueOf(10), buys.poll().getPrice());
        assertEquals(BigDecimal.valueOf(9), buys.poll().getPrice());
        assertEquals(BigDecimal.valueOf(7), buys.poll().getPrice());
    }

    @Test
    public void TestBuyOrderingSamePrice() throws Exception
    {
        var book = new OrderBook();

        book.execute(new Order("A:AUDUSD:100:10"));
        book.execute(new Order("A:AUDUSD:101:10"));
        book.execute(new Order("A:AUDUSD:102:10"));
        book.execute(new Order("A:AUDUSD:103:10"));

        var buys = book.getBuyOrders();

        assertEquals(100, buys.poll().getQuantity());
        assertEquals(101, buys.poll().getQuantity());
        assertEquals(102, buys.poll().getQuantity());
        assertEquals(103, buys.poll().getQuantity());
    }

    @Test
    public void TestSellOrdering() throws Exception
    {
        var book = new OrderBook();

        book.execute(new Order("A:AUDUSD:-100:10"));
        book.execute(new Order("A:AUDUSD:-100:7"));
        book.execute(new Order("A:AUDUSD:-100:11"));
        book.execute(new Order("A:AUDUSD:-100:9"));

        var sells = book.getSellOrders();

        assertEquals(BigDecimal.valueOf(7), sells.poll().getPrice());
        assertEquals(BigDecimal.valueOf(9), sells.poll().getPrice());
        assertEquals(BigDecimal.valueOf(10), sells.poll().getPrice());
        assertEquals(BigDecimal.valueOf(11), sells.poll().getPrice());
    }

    @Test
    public void TestSellOrderingSamePrice() throws Exception
    {
        var book = new OrderBook();

        book.execute(new Order("A:AUDUSD:-100:10"));
        book.execute(new Order("A:AUDUSD:-101:10"));
        book.execute(new Order("A:AUDUSD:-102:10"));
        book.execute(new Order("A:AUDUSD:-103:10"));

        var sells = book.getSellOrders();

        assertEquals(-100, sells.poll().getQuantity());
        assertEquals(-101, sells.poll().getQuantity());
        assertEquals(-102, sells.poll().getQuantity());
        assertEquals(-103, sells.poll().getQuantity());
    }

}