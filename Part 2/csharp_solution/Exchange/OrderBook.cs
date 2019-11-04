using System;
using System.Collections.Generic;
using System.Linq;
using Medallion.Collections;

namespace Exchange
{
    class BuyComparer : IComparer<Order>
    {
        public int Compare(Order left, Order right)
        {
            if (left.Price == right.Price) {
                return left.Generation.CompareTo(right.Generation);
            }

            return right.Price.CompareTo(left.Price);
        }
    }

    class SellComparer : IComparer<Order>
    {
        public int Compare(Order left, Order right)
        {
            if (left.Price == right.Price) {
                return left.Generation.CompareTo(right.Generation);
            }

            return left.Price.CompareTo(right.Price);
        }
    }

    public class OrderBook
    {
        public Trade[] Execute(Order order)
        {
            if (order.Quantity > 0) {
                BuyOrders.Enqueue(order);
            }    
            else {
                SellOrders.Enqueue(order);
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

        public PriorityQueue<Order> BuyOrders = new PriorityQueue<Order>(new BuyComparer());
        public PriorityQueue<Order> SellOrders = new PriorityQueue<Order>(new SellComparer());

    }
}