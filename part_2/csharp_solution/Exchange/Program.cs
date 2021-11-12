using System;
using System.IO;
using System.Text;

try {
    var exchange = new Exchange.Exchange();
    using var stream = Console.OpenStandardInput();
    using var reader = new StreamReader(stream, encoding: Encoding.ASCII, bufferSize: 1024 * 4);
    while (true) {
        var line = reader.ReadLine();
        if (string.IsNullOrEmpty(line)) {
            break;
        }
        var order = new Exchange.Order(line);
        foreach (var trade in exchange.Execute(order)) {
            Console.WriteLine(trade.Buyer + ":" + trade.Seller + ":" + trade.Instrument + ":" + trade.Quantity + ":" + trade.Price);
        }
    }
}   
catch (Exception ex) {
    Console.Error.WriteLine(ex.Message);
    Environment.Exit(1);
} 
