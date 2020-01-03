using System;

namespace Exchange
{
    public struct Trade
    {
        public Trade(string buyer, string seller, string instrument, long quantity, double price)
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
        public double Price { get; }
    }
}