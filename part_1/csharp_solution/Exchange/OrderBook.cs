using System;
using System.Collections.Generic;
using System.Linq;

namespace Exchange;

class BuyComparer : IComparer<(decimal, long)>
{
    public int Compare((decimal, long) left, (decimal, long) right)
    {
        if (left.Item1 == right.Item1) {
            return left.Item2.CompareTo(right.Item2);
        }

        return right.Item1.CompareTo(left.Item1);
    }
}

public class OrderBook
{
    public Trade[] Execute(Order order)
    {
        if (order.Quantity > 0) {
            BuyOrders.Add((order.Price, order.Generation), order);
        }    
        else {
            SellOrders.Add((order.Price, order.Generation), order);
        }

        var trades = new List<Trade>();

        while (BuyOrders.Count > 0 && SellOrders.Count > 0) {

            var ((buyPrice, buyGeneration), buy) = BuyOrders.First();
            var ((sellPrice, sellGeneration), sell) = SellOrders.First();
        
            if (buyPrice < sellPrice) {
                break;
            }

            var quantity = Math.Min((buy.RemainingQuantity), sell.RemainingQuantity);
            var price = buyGeneration < sellGeneration ? buyPrice : sellPrice;

            var trade = new Trade(buy.Participant, sell.Participant, buy.Instrument, quantity, price);

            trades.Add(trade);

            buy.Fill(quantity);
            sell.Fill(quantity);

            if (buy.RemainingQuantity == 0) {
                BuyOrders.Remove((buyPrice, buyGeneration));
            }

            if (sell.RemainingQuantity == 0) {
                SellOrders.Remove((sellPrice, sellGeneration));
            }
        }

        return trades.ToArray();
    }

    public SortedDictionary<(decimal, long), Order> BuyOrders { get; } = new(new BuyComparer());
    public SortedDictionary<(decimal, long), Order> SellOrders { get; } = new();         
}
