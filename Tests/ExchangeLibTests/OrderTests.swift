import XCTest
@testable import ExchangeLib

class OrderTests : XCTestCase
{
    func testDefaultInit() throws
    {
        let order = try Order()

        XCTAssertEqual(order.participant, "")
        XCTAssertEqual(order.instrument, "")
        XCTAssertEqual(order.quantity, 0)
        XCTAssertEqual(order.price, 0.0)
    }

    func testStringInit() throws
    {
        let order = try Order(fromString: "A:AUDUSD:100:1.47")

        XCTAssertEqual(order.participant, "A")
        XCTAssertEqual(order.instrument, "AUDUSD")
        XCTAssertEqual(order.quantity, 100)
        XCTAssertEqual(order.price, 1.47)
    }

    func testEmptyInit() throws
    {
        XCTAssertThrowsError(try Order(fromString: ""))
    }

    func testInvalidInit() throws
    {
        XCTAssertThrowsError(try Order(fromString: "blah"))
        XCTAssertThrowsError(try Order(fromString: "A:B:C:D"))
        XCTAssertThrowsError(try Order(fromString: "A:B:1.2345:1.0"))
    }

    func testWhitespaceInit() throws
    {
        let order = try Order(fromString: "  A : AUDUSD   : 100  : 1.47   ")

        XCTAssertEqual(order.participant, "A")
        XCTAssertEqual(order.instrument, "AUDUSD")
        XCTAssertEqual(order.quantity, 100)
        XCTAssertEqual(order.price, 1.47)
    }

    static var allTests =
    [
        ("testDefaultInit", testDefaultInit),
        ("testStringInit", testStringInit),
        ("testEmptyInit", testEmptyInit),
        ("testInvalidInit", testInvalidInit),
        ("testWhitespaceInit", testWhitespaceInit),
    ]
}
