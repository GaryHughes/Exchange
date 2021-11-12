using System;

namespace Exchange;

public record class Trade(string Buyer, string Seller, string Instrument, long Quantity, decimal Price);
