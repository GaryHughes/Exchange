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

    func testScenario1()
    {
        let exchange = Exchange()

        var trades : [Trade] = []
        trades += exchange.insert(order: Order(fromString: "A:AUDUSD:100:1.47"))
        trades += exchange.insert(order: Order(fromString: "B:AUDUSD:-50:1.45"))

        XCTAssertEqual(trades.count, 1)
        XCTAssertEqual(trades[0].toString(), "A:B:AUDUSD:50:1.47")
    }

    func testScenario2()
    {
        let exchange = Exchange()

        let orders =
        [
            Order(fromString: "A:GBPUSD:100:1.66"),
            Order(fromString: "B:EURUSD:-100:1.11"),
            Order(fromString: "F:EURUSD:-50:1.1"),
            Order(fromString: "C:GBPUSD:-10:1.5"),
            Order(fromString: "C:GBPUSD:-20:1.6"),
            Order(fromString: "C:GBPUSD:-20:1.7"),
            Order(fromString: "D:EURUSD:100:1.11")
        ]

        var trades : [Trade] = []
        for order in orders
        {
            trades += exchange.insert(order: order)
        }

        XCTAssertEqual(trades.count, 4)
        XCTAssertEqual(trades[0].toString(), "A:C:GBPUSD:10:1.66")
        XCTAssertEqual(trades[1].toString(), "A:C:GBPUSD:20:1.66")
        XCTAssertEqual(trades[2].toString(), "D:F:EURUSD:50:1.1")
        XCTAssertEqual(trades[3].toString(), "D:B:EURUSD:50:1.11")
    }

    static var allTests =
    [
        ("testDefaultInit", testDefaultInit),
        ("testInsert", testInsert),
        ("testInsertMultiple", testInsertMultiple),
        ("testTrade", testTrade),
        ("testScenario1", testScenario1),
        ("testScenario2", testScenario2),
    ]
}
