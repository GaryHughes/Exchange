import XCTest
@testable import ExchangeLib

class OrderTests : XCTestCase
{
    func testStringInit() throws
    {
        let order = try Order(fromString: "A:AUDUSD:100:1.47")!

        XCTAssertEqual(order.participant, "A")
        XCTAssertEqual(order.instrument, "AUDUSD")
        XCTAssertEqual(order.quantity, 100)
        XCTAssertEqual(order.price, 1.47)
    }

    func testEmptyInit() throws
    {
        XCTAssertNil(try Order(fromString: ""))
    }

    func testInvalidInit() throws
    {
        XCTAssertNil(try Order(fromString: "blah"))
        XCTAssertNil(try Order(fromString: "A:B:C:D"))
        XCTAssertNil(try Order(fromString: "A:B:1.2345:1.0"))
    }

    func testWhitespaceInit() throws
    {
        let order = try Order(fromString: "  A : AUDUSD   : 100  : 1.47   ")!

        XCTAssertEqual(order.participant, "A")
        XCTAssertEqual(order.instrument, "AUDUSD")
        XCTAssertEqual(order.quantity, 100)
        XCTAssertEqual(order.price, 1.47)
    }
   
}
