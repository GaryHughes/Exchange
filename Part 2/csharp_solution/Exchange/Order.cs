using System;
using System.Collections.Generic;

namespace Exchange
{
    public class Order
    {
        enum Token {
            Participant,
            Instrument,
            Quantity,
            Price
            
        };

        static readonly Dictionary<int, string> _strings = new Dictionary<int, string>();

        static string GetString(ReadOnlySpan<char> span)
        {
            var hash = string.GetHashCode(span);
            if (!_strings.TryGetValue(hash, out var value))
            {
                value = span.ToString();
                _strings[hash] = value;
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
                        Participant = GetString(input.Slice(start, end - start));
                        token = Token.Instrument;
                        break;
                    case Token.Instrument:
                        Instrument = GetString(input.Slice(start, end - start));
                        token = Token.Quantity;
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

            Price = double.Parse(input.Slice(start));

            RemainingQuantity = Math.Abs(Quantity);
            Generation = NextGeneration++;
        }

        public string Participant { get; } 
        public string Instrument { get; }
        public long Quantity { get; }
        public long RemainingQuantity { get; private set; }
        public double Price { get; }
        public int Generation { get; }

        public void Fill(long quantity)
        {
            RemainingQuantity -= quantity;
        }
        
        private static int NextGeneration = 1;

        public override string ToString() {
            return $"{Participant}:{Instrument}:{Quantity}:{Price}";
        }
    }    
}