import XCTest
@testable import ExchangeLib

class TradeTests : XCTestCase
{
    func testDefaultInit()
    {
        let trade = Trade()

        XCTAssertEqual(trade.buyer, "")
        XCTAssertEqual(trade.seller, "")
        XCTAssertEqual(trade.instrument, "")
        XCTAssertEqual(trade.quantity, 0)
        XCTAssertEqual(trade.price, 0.0)
    }

    func testToString()
    {
        let trade = Trade()
        trade.buyer = "A"
        trade.seller = "B"
        trade.instrument = "AUDUSD"
        trade.quantity = 50
        trade.price = 1.47

        XCTAssertEqual(trade.toString(), "A:B:AUDUSD:50:1.47")
    }

    static var allTests =
    [
        ("testDefaultInit", testDefaultInit),
        ("testToString", testToString),
    ]
}
