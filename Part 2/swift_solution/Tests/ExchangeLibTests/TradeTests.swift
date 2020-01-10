import XCTest
@testable import ExchangeLib

class TradeTests : XCTestCase
{
    func testDefaultInit()
    {
        let trade = Trade(buyer: "", seller: "", instrument: "", quantity: 0, price: 0.0)

        XCTAssertEqual(trade.buyer, "")
        XCTAssertEqual(trade.seller, "")
        XCTAssertEqual(trade.instrument, "")
        XCTAssertEqual(trade.quantity, 0)
        XCTAssertEqual(trade.price, 0.0)
    }

    func testToString()
    {
        let trade = Trade(buyer: "A", seller: "B", instrument: "AUDUSD", quantity: 50, price: 1.47)
        XCTAssertEqual(trade.toString(), "A:B:AUDUSD:50:1.47")
    }

}
