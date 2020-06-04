using System;

namespace Exchange
{
    public class Trade
    {
        public Trade(string buyer, string seller, string instrument, long quantity, decimal price)
        {
            Buyer = buyer;
            Seller = seller;
            Instrument = instrument;
            Quantity = quantity;
            Price = price;
        }

        public string Buyer { get; }
        public string Seller { get; }
        public string Instrument { get; }
        public long Quantity { get; }
        public decimal Price { get; }
    }
}