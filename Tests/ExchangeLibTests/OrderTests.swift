import XCTest
@testable import ExchangeLib

class OrderTests : XCTestCase
{
    func testDefaultInit()
    {
        let order = Order()

        XCTAssertEqual(order.trader, "")
        XCTAssertEqual(order.instrument, "")
        XCTAssertEqual(order.quantity, 0.0)
        XCTAssertEqual(order.price, 0.0)
    }

    func testStringInit()
    {
        let order = Order(fromString: "A:AUDUSD:100:1.47")

        XCTAssertEqual(order.trader, "A")
        XCTAssertEqual(order.instrument, "AUDUSD")
        XCTAssertEqual(order.quantity, 100.0)
        XCTAssertEqual(order.price, 1.47)
    }

    static var allTests =
    [
        ("testDefaultInit", testDefaultInit),
        ("testStringInit", testStringInit),
    ]
}
