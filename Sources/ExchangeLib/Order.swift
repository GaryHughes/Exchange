import Foundation

public class Order
{
    var trader = ""
    var instrument = ""
    var quantity = 0
    var price = 0.0

    public init()
    {
    }

    public init(fromString string: String)
    {
        // <buyer/sellerid>:<instrument>:<signed­quantity>:<limit­price>
        // eg. A:AUDUSD:­100:1.47

        let elements =  string.components(separatedBy: ":")
        trader = elements[0]
        instrument = elements[1]
        quantity = Int(elements[2]) ?? 0
        price = Double(elements[3]) ?? 0.0
    }
}

