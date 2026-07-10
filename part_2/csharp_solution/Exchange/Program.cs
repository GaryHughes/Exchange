using System;
using System.IO;
using System.Text;

// Console.Out autoflushes on every WriteLine, even when redirected to a file, costing one
// flush syscall per trade. Buffer stdout ourselves and only flush at the end (or explicitly
// on the error path below, since Environment.Exit skips any pending `using` disposal).
using var writer = new StreamWriter(Console.OpenStandardOutput(), Encoding.ASCII, bufferSize: 1024 * 64) { AutoFlush = false };

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
            // Write fields directly instead of concatenating with '+', which would box
            // Quantity/Price and build an intermediate string per trade for no benefit
            // here - the writer already buffers the output.
            writer.Write(trade.Buyer);
            writer.Write(':');
            writer.Write(trade.Seller);
            writer.Write(':');
            writer.Write(trade.Instrument);
            writer.Write(':');
            writer.Write(trade.Quantity);
            writer.Write(':');
            writer.Write(trade.Price);
            writer.WriteLine();
        }
    }
}
catch (Exception ex) {
    writer.Flush();
    Console.Error.WriteLine(ex.Message);
    Environment.Exit(1);
}
