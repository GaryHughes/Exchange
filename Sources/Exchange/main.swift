import ExchangeLib

let exchange = Exchange()

var trades : [Trade] = []
while let line = readLine()
{
    do
    {
        var order = try Order(fromString: line)
        trades += exchange.insert(order: order)
    }
    catch let error
    {
        print("Error while processing orders: \(error)")
    }
}

for trade in trades
{
    print(trade.toString())
}

