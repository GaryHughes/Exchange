import ExchangeLib

let exchange = Exchange()

while let line = readLine()
{
    do
    {
        let trimmed = line.trimmingCharacters(in: .whitespacesAndNewlines)
        if trimmed.count == 0 {
            continue
        }
        let order = try Order(fromString: trimmed)
        for trade in exchange.insert(order: order) {
            print(trade.toString())
        }
    }
    catch let error
    {
        print("Error while processing orders: \(error)")
    }
}


