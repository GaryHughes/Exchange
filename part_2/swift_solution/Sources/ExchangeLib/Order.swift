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

    // Faster path for the hot stdin-reading loop: avoids String.components(separatedBy:),
    // which splits at the (Unicode) grapheme-cluster level and is a well known bottleneck
    // for high-volume text processing in Swift. Behaves like fromString: at least 4
    // colon-delimited fields are required, and anything after the 4th is ignored.
    public init?(fromBytes bytes: Data) throws
    {
        guard let (participantBytes, instrumentBytes, quantityBytes, priceBytes) = Order.fields(from: bytes) else
        {
            return nil
        }

        let quantityString = String(decoding: quantityBytes, as: UTF8.self)
        guard let parsedQuantity = Int(quantityString) else
        {
            throw OrderError.InvalidQuantity(quantityString)
        }

        let priceString = String(decoding: priceBytes, as: UTF8.self)
        guard let parsedPrice = Double(priceString) else
        {
            throw OrderError.InvalidPrice(priceString)
        }

        participant = String(decoding: participantBytes, as: UTF8.self)
        instrument = String(decoding: instrumentBytes, as: UTF8.self)
        quantity = parsedQuantity
        remainingQuantity = abs(quantity)
        price = parsedPrice
        generation = Order.allocateGeneration()
    }

    private static func fields(from bytes: Data) -> (Data, Data, Data, Data)?
    {
        let colon: UInt8 = 0x3A

        guard let firstColon = bytes.firstIndex(of: colon) else { return nil }
        guard let secondColon = bytes[bytes.index(after: firstColon)...].firstIndex(of: colon) else { return nil }
        guard let thirdColon = bytes[bytes.index(after: secondColon)...].firstIndex(of: colon) else { return nil }
        let fourthColon = bytes[bytes.index(after: thirdColon)...].firstIndex(of: colon)

        return (
            bytes[bytes.startIndex..<firstColon],
            bytes[bytes.index(after: firstColon)..<secondColon],
            bytes[bytes.index(after: secondColon)..<thirdColon],
            bytes[bytes.index(after: thirdColon)..<(fourthColon ?? bytes.endIndex)]
        )
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
