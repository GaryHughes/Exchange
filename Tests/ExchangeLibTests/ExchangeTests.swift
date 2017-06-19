import XCTest
@testable import ExchangeLib

class ExchangeTests : XCTestCase
{
    func testDefaultInit()
    {
        let exchange = Exchange()

        XCTAssertEqual(exchange.orderBooks.count, 0)
    }

    func testInsert()
    {
        let exchange = Exchange()

        let trades = exchange.insert(order: Order(fromString: "A:AUDUSD:100:1.47"))

        XCTAssertEqual(trades.count, 0)
        XCTAssertEqual(exchange.orderBooks.count, 1)
    }

    func testInsertMultiple()
    {
        let exchange = Exchange()

        var trades : [Trade] = []
        trades += exchange.insert(order: Order(fromString: "A:AUDUSD:100:1.47"))
        trades += exchange.insert(order: Order(fromString: "B:GBPUSD:100:1.47"))
        trades += exchange.insert(order: Order(fromString: "C:USDCAD:100:1.47"))

        XCTAssertEqual(trades.count, 0)
        XCTAssertEqual(exchange.orderBooks.count, 3)
    }

    func testTrade()
    {
        let exchange = Exchange()

        var trades : [Trade] = []
        trades += exchange.insert(order: Order(fromString: "A:AUDUSD:100:1.47"))
        trades += exchange.insert(order: Order(fromString: "B:AUDUSD:-100:1.47"))

        XCTAssertEqual(exchange.orderBooks.count, 1)
        XCTAssertEqual(trades.count, 1)
    }

    static var allTests =
    [
        ("testDefaultInit", testDefaultInit),
        ("testInsert", testInsert),
        ("testInsertMultiple", testInsertMultiple),
        ("testTrade", testTrade),
    ]
}
