import Foundation

public class Order
{
    let participant: String
    let instrument: String
    let quantity: Int
    var remainingQuantity: Int
    let price: Double
    let generation: Int
    private static var nextGeneration: Int = 1
    
    public init?(fromString string: String) throws
    {
        // <buyer/sellerid>:<instrument>:<signed quantity>:<limit price>
        // eg. A:AUDUSD:100:1.47

        let elements = string.components(separatedBy: ":")
       
        if elements.count < 4
        {
            return nil
        }

        guard let parsedQuantity = Int(elements[2]) else
        {
            throw OrderError.InvalidQuantity(elements[2])
        }
        
        guard let parsedPrice = Double(elements[3]) else
        {
            throw OrderError.InvalidPrice(elements[3])
        }

        participant = elements[0]
        instrument = elements[1]
        quantity = parsedQuantity
        remainingQuantity = abs(quantity)
        price = parsedPrice
        generation = Order.allocateGeneration()
    }
    
    public func fill(amount: Int) {
        remainingQuantity -= amount;
    }
  
    private static func allocateGeneration() -> Int {
        let result = nextGeneration;
        nextGeneration = nextGeneration + 1
        return result
    }
}

public enum OrderError : Error
{
    case UnableToParse(String)
    case InvalidQuantity(String)
    case InvalidPrice(String)
}
