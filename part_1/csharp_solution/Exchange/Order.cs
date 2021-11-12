using System;

namespace Exchange;

public class Order
{
    public Order(string line)
    {
        var components = line.Split(':');
        if (components.Length != 4) {
            throw new Exception($"Invalid line '{line}' expected 4 components and found {components.Length}");
        }
        Participant = components[0];
        Instrument = components[1];
        Quantity = long.Parse(components[2]);
        RemainingQuantity = Math.Abs(Quantity);
        Price = decimal.Parse(components[3]);
        Generation = NextGeneration++;
    }

    public string Participant { get; }
    public string Instrument { get; }
    public long Quantity { get; }
    public long RemainingQuantity { get; private set; }
    public decimal Price { get; }
    public int Generation { get; }

    public void Fill(long quantity)
    {
        RemainingQuantity -= quantity;
    }
    
    private static int NextGeneration = 1;
}    
