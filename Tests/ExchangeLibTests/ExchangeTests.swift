import XCTest
@testable import ExchangeLib

class ExchangeTests : XCTestCase
{
    func testDefaultInit()
    {
        let exchange = Exchange()

        XCTAssertEqual(exchange.orderBooks.count, 0)
    }

    func testInsert() throws
    {
        let exchange = Exchange()

        exchange.insert(order: try Order(fromString: "A:AUDUSD:100:1.47"))
        exchange.waitForTrades()

        XCTAssertEqual(exchange.trades.count, 0)
        XCTAssertEqual(exchange.orderBooks.count, 1)
    }

    func testInsertMultiple() throws
    {
        let exchange = Exchange()

        exchange.insert(order: try Order(fromString: "A:AUDUSD:100:1.47"))
        exchange.insert(order: try Order(fromString: "B:GBPUSD:100:1.47"))
        exchange.insert(order: try Order(fromString: "C:USDCAD:100:1.47"))
        exchange.waitForTrades()

        XCTAssertEqual(exchange.trades.count, 0)
        XCTAssertEqual(exchange.orderBooks.count, 3)
    }

    func testTrade() throws
    {
        let exchange = Exchange()

        exchange.insert(order: try Order(fromString: "A:AUDUSD:100:1.47"))
        exchange.insert(order: try Order(fromString: "B:AUDUSD:-100:1.47"))
        exchange.waitForTrades()

        XCTAssertEqual(exchange.orderBooks.count, 1)
        XCTAssertEqual(exchange.trades.count, 1)
    }

    func testScenario1() throws
    {
        let exchange = Exchange()

        exchange.insert(order: try Order(fromString: "A:AUDUSD:100:1.47"))
        exchange.insert(order: try Order(fromString: "B:AUDUSD:-50:1.45"))
        exchange.waitForTrades()

        XCTAssertEqual(exchange.trades.count, 1)
        XCTAssertEqual(exchange.trades[0].toString(), "A:B:AUDUSD:50:1.47")
    }

    func testScenario2() throws
    {
        let exchange = Exchange()

        let orders =
        [
            try Order(fromString: "A:GBPUSD:100:1.66"),
            try Order(fromString: "B:EURUSD:-100:1.11"),
            try Order(fromString: "F:EURUSD:-50:1.1"),
            try Order(fromString: "C:GBPUSD:-10:1.5"),
            try Order(fromString: "C:GBPUSD:-20:1.6"),
            try Order(fromString: "C:GBPUSD:-20:1.7"),
            try Order(fromString: "D:EURUSD:100:1.11")
        ]

        for order in orders
        {
            exchange.insert(order: order)
        }
        exchange.waitForTrades()

        XCTAssertEqual(exchange.trades.count, 4)
        XCTAssert(exchange.trades.contains { $0.toString() == "A:C:GBPUSD:10:1.66" })
        XCTAssert(exchange.trades.contains { $0.toString() == "A:C:GBPUSD:20:1.66" })
        XCTAssert(exchange.trades.contains { $0.toString() == "D:F:EURUSD:50:1.1" })
        XCTAssert(exchange.trades.contains { $0.toString() == "D:B:EURUSD:50:1.11" })

        // TODO: The order of trades should be deterministic at least within each currency
        // XCTAssertEqual(exchange.trades[0].toString(), "A:C:GBPUSD:10:1.66")
        // XCTAssertEqual(exchange.trades[1].toString(), "A:C:GBPUSD:20:1.66")
        // XCTAssertEqual(exchange.trades[2].toString(), "D:F:EURUSD:50:1.1")
        // XCTAssertEqual(exchange.trades[3].toString(), "D:B:EURUSD:50:1.11")
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
