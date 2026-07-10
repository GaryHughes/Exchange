import Foundation
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

    func testBytesInit() throws
    {
        let order = try Order(fromBytes: "A:AUDUSD:100:1.47".data(using: .utf8)!)!

        XCTAssertEqual(order.participant, "A")
        XCTAssertEqual(order.instrument, "AUDUSD")
        XCTAssertEqual(order.quantity, 100)
        XCTAssertEqual(order.price, 1.47)
    }

    func testBytesSellInit() throws
    {
        let order = try Order(fromBytes: "A:AUDUSD:-100:1.47".data(using: .utf8)!)!

        XCTAssertEqual(order.participant, "A")
        XCTAssertEqual(order.instrument, "AUDUSD")
        XCTAssertEqual(order.quantity, -100)
        XCTAssertEqual(order.price, 1.47)
    }

    func testBytesEmptyInit() throws
    {
        XCTAssertNil(try Order(fromBytes: Data()))
    }

    func testBytesInvalidInit() throws
    {
        XCTAssertNil(try Order(fromBytes: "blah".data(using: .utf8)!))
    }

    func testBytesExtraFieldIsIgnored() throws
    {
        // Matches fromString: only the first 4 colon-delimited fields are used,
        // anything after the 4th is ignored.
        let order = try Order(fromBytes: "A:AUDUSD:100:1.47:extra".data(using: .utf8)!)!

        XCTAssertEqual(order.participant, "A")
        XCTAssertEqual(order.instrument, "AUDUSD")
        XCTAssertEqual(order.quantity, 100)
        XCTAssertEqual(order.price, 1.47)
    }

}
