
public class Trade
{
    let buyer: String
    let seller: String
    let instrument: String
    let quantity: Int
    let price: Double
    
    public init(buyer: String, seller: String, instrument: String, quantity: Int, price: Double)
    {
        self.buyer = buyer
        self.seller = seller
        self.instrument = instrument
        self.quantity = quantity
        self.price = price
    }

    public func toString() -> String
    {
        return "\(buyer):\(seller):\(instrument):\(quantity):\(price)"
    }
}
