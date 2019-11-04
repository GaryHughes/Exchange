using System;
using System.Collections.Generic;
using System.Linq;
using NGenerics.DataStructures.Queues;

namespace Exchange
{
    class BuyComparer : IComparer<(decimal, long)>
    {
        public int Compare((decimal, long) left, (decimal, long) right)
        {
            if (left.Item1 == right.Item1) {
                return right.Item2.CompareTo(left.Item2);
            }

            return left.Item1.CompareTo(right.Item1);
        }
    }

    public class OrderBook
    {
        public Trade[] Execute(Order order)
        {
            if (order.Quantity > 0) {
                BuyOrders.Add(order, (order.Price, order.Generation));
            }    
            else {
                SellOrders.Add(order, (order.Price, order.Generation));
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

        public PriorityQueue<Order, (decimal, long)> BuyOrders = new PriorityQueue<Order, (decimal, long)>(PriorityQueueType.Maximum, new BuyComparer());
        public PriorityQueue<Order, (decimal, long)> SellOrders = new PriorityQueue<Order, (decimal, long)>(PriorityQueueType.Minimum);
    
    }
}