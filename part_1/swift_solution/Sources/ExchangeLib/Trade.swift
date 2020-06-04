
public class Trade
{
    var buyer = ""
    var seller = ""
    var instrument = ""
    var quantity = 0
    var price = 0.0

    public func toString() -> String
    {
        return "\(buyer):\(seller):\(instrument):\(quantity):\(price)"
    }
}
