using System;

namespace Exchange;

public record struct Trade(string Buyer, string Seller, string Instrument, long Quantity, double Price);
