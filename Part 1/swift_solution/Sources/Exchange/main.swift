import ExchangeLib

let exchange = Exchange()

while let line = readLine()
{
    do
    {
        let order = try Order(fromString: line)
        exchange.insert(order: order)
    }
    catch let error
    {
        print("Error while processing orders: \(error)")
    }
}

exchange.waitForTrades()
for trade in exchange.trades
{
    print(trade.toString())
}

