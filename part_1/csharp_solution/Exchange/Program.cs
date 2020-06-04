using System;
using System.IO;

namespace Exchange
{
    class Program
    {
        static void Main(string[] args)
        {
            try {
                var exchange = new Exchange();
                while (true) {
                    var line = Console.ReadLine();
                    if (string.IsNullOrEmpty(line)) {
                        break;
                    }
                    var order = new Order(line);
                    foreach (var trade in exchange.Execute(order)) {
                        Console.WriteLine($"{trade.Buyer}:{trade.Seller}:{trade.Instrument}:{trade.Quantity}:{trade.Price}");
                    }
                }
            }   
            catch (Exception ex) {
                Console.Error.WriteLine(ex.Message);
                Environment.Exit(1);
            } 
        }
    }
}
