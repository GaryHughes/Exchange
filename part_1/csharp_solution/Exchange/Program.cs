using System;

try {
    var exchange = new Exchange.Exchange();
    while (true) {
        var line = Console.ReadLine();
        if (string.IsNullOrEmpty(line)) {
            break;
        }
        var order = new Exchange.Order(line);
        foreach (var trade in exchange.Execute(order)) {
            Console.WriteLine($"{trade.Buyer}:{trade.Seller}:{trade.Instrument}:{trade.Quantity}:{trade.Price}");
        }
    }
}   
catch (Exception ex) {
    Console.Error.WriteLine(ex.Message);
    Environment.Exit(1);
} 
