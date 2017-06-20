import ExchangeLib

let exchange = Exchange()

var trades : [Trade] = []
while let line = readLine()
{
    var order = Order(fromString: line)
    trades += exchange.insert(order: order)
}

for trade in trades
{
    print(trade.toString())
}

