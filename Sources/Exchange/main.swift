import ExchangeLib

let exchange = Exchange()

while let line = readLine()
{
    do
    {
        var order = try Order(fromString: line)

        let trades = exchange.insert(order: order)
        for trade in trades
        {
            print(trade.toString())
        }
    }
    catch let error
    {
        print("Error while processing orders: \(error)")
    }
}


