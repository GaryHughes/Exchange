using System;
using System.Collections.Generic;
using System.Linq;

namespace Exchange;

class BuyComparer : IComparer<Order.PriorityKey>
{
    public int Compare(Order.PriorityKey left, Order.PriorityKey right)
    {
        var result = right.Price.CompareTo(left.Price);

        if (result == 0) {
            return left.Generation.CompareTo(right.Generation);
        }

        return result;
    }
}

class SellComparer : IComparer<Order.PriorityKey>
{
    public int Compare(Order.PriorityKey left, Order.PriorityKey right)
    {
        var result = left.Price.CompareTo(right.Price);

        if (result == 0) {
            return left.Generation.CompareTo(right.Generation);
        }

        return result;
    }
}

public class OrderBook
{
    public Trade[] Execute(Order order)
    {
        if (order.Quantity > 0) {
            BuyOrders.Enqueue(order, order.Priority);
        }    
        else {
            SellOrders.Enqueue(order, order.Priority);
        }

        var trades = new List<Trade>();

        while (BuyOrders.Count > 0 && SellOrders.Count > 0) {

            var buy = BuyOrders.Peek();
            var sell = SellOrders.Peek();

            if (buy.Price < sell.Price) {
                break;
            }

            var quantity = Math.Min((buy.RemainingQuantity), sell.RemainingQuantity);
            var price = buy.Generation < sell.Generation ? buy.Price : sell.Price;

            var trade = new Trade(buy.Participant, sell.Participant, buy.Instrument, quantity, price);

            trades.Add(trade);

            buy.Fill(quantity);
            sell.Fill(quantity);

            if (buy.RemainingQuantity == 0) {
                BuyOrders.Dequeue();
            }

            if (sell.RemainingQuantity == 0) {
                SellOrders.Dequeue();
            }
        }
        
        return trades.ToArray();
    }

    public PriorityQueue<Order, Order.PriorityKey> BuyOrders = new (new BuyComparer());
    public PriorityQueue<Order, Order.PriorityKey> SellOrders = new (new SellComparer());

}
