using System;
using System.Collections.Generic;

namespace Exchange;

public class Order
{
    enum Token {
        Participant,
        Instrument,
        Quantity,
        Price
        
    };

    static readonly Dictionary<int, string> Strings = [];

    static string GetString(ref ReadOnlySpan<char> span)
    {
        var hash = string.GetHashCode(span);
        if (!Strings.TryGetValue(hash, out var value))
        {
            value = span.ToString();
            Strings[hash] = value;
        }
        return value;
    }
    
    public Order(string line)
    {
        Quantity = 0;
        Participant = "";
        Instrument = "";
        
        var input = line.AsSpan();
        var token = Token.Participant;
        int start = 0;
        int end = 0;
        for (start = 0; end < input.Length; ++end) {
            if (input[end] != ':') {
                continue;
            }
            switch (token) {
                case Token.Participant:
                    {
                    var span = input.Slice(start, end - start);
                    Participant = GetString(ref span);
                    token = Token.Instrument;
                    }
                    break;
                case Token.Instrument:
                    {
                    var span = input.Slice(start, end - start);
                    Instrument = GetString(ref span);
                    token = Token.Quantity;
                    }
                    break;
                case Token.Quantity:
                    Quantity = long.Parse(input.Slice(start, end - start));
                    token = Token.Price;
                    break;
            }
            start = end + 1;
        }

        if (token != Token.Price) {
            throw new ArgumentException($"Could not parse order '{line}'", nameof(line));
        }

        RemainingQuantity = Math.Abs(Quantity);

        Priority = new PriorityKey(double.Parse(input.Slice(start)), NextGeneration++);
    }

    public readonly record struct PriorityKey(double Price, int Generation);

    public PriorityKey Priority { get; init; }

    public string Participant { get; } 
    public string Instrument { get; }
    public long Quantity { get; }
    public long RemainingQuantity { get; private set; }
    public double Price => Priority.Price;
    public int Generation => Priority.Generation;
    

    public void Fill(long quantity)
    {
        RemainingQuantity -= quantity;
    }
    
    private static int NextGeneration = 1;

    public override string ToString() {
        return $"{Participant}:{Instrument}:{Quantity}:{Price}";
    }
}    
