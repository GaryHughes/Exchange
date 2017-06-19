import XCTest
@testable import ExchangeLib

class OrderBookTests : XCTestCase
{
    func testInit()
    {
        let orderBook = OrderBook()

        XCTAssertEqual(orderBook.bids.count, 0)
        XCTAssertEqual(orderBook.asks.count, 0)
    }

    func testInsertBid()
    {
        let orderBook = OrderBook()

        let trades = orderBook.insert(order: Order(fromString: "A:AUDUSD:100:1.47"))

        XCTAssertEqual(orderBook.bids.count, 1)
        XCTAssertEqual(orderBook.asks.count, 0)
        XCTAssertEqual(trades.count, 0)

        XCTAssertEqual(orderBook.bids[0].trader, "A")
        XCTAssertEqual(orderBook.bids[0].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.bids[0].quantity, 100)
        XCTAssertEqual(orderBook.bids[0].price, 1.47)
    }

    func testInsertAsk()
    {
        let orderBook = OrderBook()

        let trades = orderBook.insert(order: Order(fromString: "B:AUDUSD:-100:1.47"))

        XCTAssertEqual(orderBook.bids.count, 0)
        XCTAssertEqual(orderBook.asks.count, 1)
        XCTAssertEqual(trades.count, 0)

        XCTAssertEqual(orderBook.asks[0].trader, "B")
        XCTAssertEqual(orderBook.asks[0].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.asks[0].quantity, -100)
        XCTAssertEqual(orderBook.asks[0].price, 1.47)
    }

    func testInsertBidsAtDifferentPrices()
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.insert(order: Order(fromString: "A:AUDUSD:100:1.47"))
        trades += orderBook.insert(order: Order(fromString: "B:AUDUSD:200:1.48"))
        trades += orderBook.insert(order: Order(fromString: "C:AUDUSD:300:1.49"))

        XCTAssertEqual(orderBook.bids.count, 3)
        XCTAssertEqual(orderBook.asks.count, 0)
        XCTAssertEqual(trades.count, 0)

        XCTAssertEqual(orderBook.bids[0].trader, "C")
        XCTAssertEqual(orderBook.bids[0].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.bids[0].quantity, 300)
        XCTAssertEqual(orderBook.bids[0].price, 1.49)

        XCTAssertEqual(orderBook.bids[1].trader, "B")
        XCTAssertEqual(orderBook.bids[1].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.bids[1].quantity, 200)
        XCTAssertEqual(orderBook.bids[1].price, 1.48)

        XCTAssertEqual(orderBook.bids[2].trader, "A")
        XCTAssertEqual(orderBook.bids[2].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.bids[2].quantity, 100)
        XCTAssertEqual(orderBook.bids[2].price, 1.47)
    }

    func testInsertAsksAtDifferentPrices()
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.insert(order: Order(fromString: "A:AUDUSD:-100:1.49"))
        trades += orderBook.insert(order: Order(fromString: "B:AUDUSD:-200:1.48"))
        trades += orderBook.insert(order: Order(fromString: "C:AUDUSD:-300:1.47"))

        XCTAssertEqual(orderBook.bids.count, 0)
        XCTAssertEqual(orderBook.asks.count, 3)
        XCTAssertEqual(trades.count, 0)

        XCTAssertEqual(orderBook.asks[0].trader, "C")
        XCTAssertEqual(orderBook.asks[0].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.asks[0].quantity, -300)
        XCTAssertEqual(orderBook.asks[0].price, 1.47)

        XCTAssertEqual(orderBook.asks[1].trader, "B")
        XCTAssertEqual(orderBook.asks[1].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.asks[1].quantity, -200)
        XCTAssertEqual(orderBook.asks[1].price, 1.48)

        XCTAssertEqual(orderBook.asks[2].trader, "A")
        XCTAssertEqual(orderBook.asks[2].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.asks[2].quantity, -100)
        XCTAssertEqual(orderBook.asks[2].price, 1.49)
    }

    func testInsertBidsAtTheSamePrice()
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.insert(order: Order(fromString: "A:AUDUSD:100:1.47"))
        trades += orderBook.insert(order: Order(fromString: "B:AUDUSD:200:1.47"))
        trades += orderBook.insert(order: Order(fromString: "C:AUDUSD:300:1.47"))

        XCTAssertEqual(orderBook.bids.count, 3)
        XCTAssertEqual(orderBook.asks.count, 0)
        XCTAssertEqual(trades.count, 0)

        XCTAssertEqual(orderBook.bids[0].trader, "A")
        XCTAssertEqual(orderBook.bids[0].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.bids[0].quantity, 100)
        XCTAssertEqual(orderBook.bids[0].price, 1.47)

        XCTAssertEqual(orderBook.bids[1].trader, "B")
        XCTAssertEqual(orderBook.bids[1].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.bids[1].quantity, 200)
        XCTAssertEqual(orderBook.bids[1].price, 1.47)

        XCTAssertEqual(orderBook.bids[2].trader, "C")
        XCTAssertEqual(orderBook.bids[2].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.bids[2].quantity, 300)
        XCTAssertEqual(orderBook.bids[2].price, 1.47)
    }

    func testInsertAsksAtTheSamePrice()
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.insert(order: Order(fromString: "A:AUDUSD:-100:1.47"))
        trades += orderBook.insert(order: Order(fromString: "B:AUDUSD:-200:1.47"))
        trades += orderBook.insert(order: Order(fromString: "C:AUDUSD:-300:1.47"))

        XCTAssertEqual(orderBook.bids.count, 0)
        XCTAssertEqual(orderBook.asks.count, 3)
        XCTAssertEqual(trades.count, 0)

        XCTAssertEqual(orderBook.asks[0].trader, "A")
        XCTAssertEqual(orderBook.asks[0].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.asks[0].quantity, -100)
        XCTAssertEqual(orderBook.asks[0].price, 1.47)

        XCTAssertEqual(orderBook.asks[1].trader, "B")
        XCTAssertEqual(orderBook.asks[1].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.asks[1].quantity, -200)
        XCTAssertEqual(orderBook.asks[1].price, 1.47)

        XCTAssertEqual(orderBook.asks[2].trader, "C")
        XCTAssertEqual(orderBook.asks[2].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.asks[2].quantity, -300)
        XCTAssertEqual(orderBook.asks[2].price, 1.47)
    }

    func testMatchNothing()
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.insert(order: Order(fromString: "A:AUDUSD:100:1.47"))
        trades += orderBook.insert(order: Order(fromString: "B:AUDUSD:-100:1.48"))

        XCTAssertEqual(orderBook.bids.count, 1)
        XCTAssertEqual(orderBook.asks.count, 1)
        XCTAssertEqual(trades.count, 0)

        XCTAssertEqual(orderBook.bids[0].trader, "A")
        XCTAssertEqual(orderBook.bids[0].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.bids[0].quantity, 100)
        XCTAssertEqual(orderBook.bids[0].price, 1.47)

        XCTAssertEqual(orderBook.asks[0].trader, "B")
        XCTAssertEqual(orderBook.asks[0].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.asks[0].quantity, -100)
        XCTAssertEqual(orderBook.asks[0].price, 1.48)
    }

    func testMatchBidExactly()
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.insert(order: Order(fromString: "A:AUDUSD:100:1.47"))
        trades += orderBook.insert(order: Order(fromString: "B:AUDUSD:-100:1.47"))

        XCTAssertEqual(orderBook.bids.count, 0)
        XCTAssertEqual(orderBook.asks.count, 0)
        XCTAssertEqual(trades.count, 1)

        XCTAssertEqual(trades[0].buyer, "A")
        XCTAssertEqual(trades[0].seller, "B")
        XCTAssertEqual(trades[0].instrument, "AUDUSD")
        XCTAssertEqual(trades[0].quantity, 100)
        XCTAssertEqual(trades[0].price, 1.47)

    }

    func testMatchAskExactly()
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.insert(order: Order(fromString: "A:AUDUSD:-100:1.47"))
        trades += orderBook.insert(order: Order(fromString: "B:AUDUSD:100:1.47"))

        XCTAssertEqual(orderBook.bids.count, 0)
        XCTAssertEqual(orderBook.asks.count, 0)
        XCTAssertEqual(trades.count, 1)

        XCTAssertEqual(trades[0].buyer, "B")
        XCTAssertEqual(trades[0].seller, "A")
        XCTAssertEqual(trades[0].instrument, "AUDUSD")
        XCTAssertEqual(trades[0].quantity, 100)
        XCTAssertEqual(trades[0].price, 1.47)
    }

    func testMatchBidPartial()
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.insert(order: Order(fromString: "A:AUDUSD:250:1.47"))
        trades += orderBook.insert(order: Order(fromString: "B:AUDUSD:-100:1.47"))

        XCTAssertEqual(orderBook.bids.count, 1)
        XCTAssertEqual(orderBook.asks.count, 0)
        XCTAssertEqual(trades.count, 1)

        XCTAssertEqual(trades[0].buyer, "A")
        XCTAssertEqual(trades[0].seller, "B")
        XCTAssertEqual(trades[0].instrument, "AUDUSD")
        XCTAssertEqual(trades[0].quantity, 100)
        XCTAssertEqual(trades[0].price, 1.47)

        XCTAssertEqual(orderBook.bids[0].trader, "A")
        XCTAssertEqual(orderBook.bids[0].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.bids[0].quantity, 150)
        XCTAssertEqual(orderBook.bids[0].price, 1.47)
    }

    func testMatchAskPartial()
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.insert(order: Order(fromString: "A:AUDUSD:-250:1.47"))
        trades += orderBook.insert(order: Order(fromString: "B:AUDUSD:100:1.47"))

        XCTAssertEqual(orderBook.bids.count, 0)
        XCTAssertEqual(orderBook.asks.count, 1)
        XCTAssertEqual(trades.count, 1)

        XCTAssertEqual(trades[0].buyer, "B")
        XCTAssertEqual(trades[0].seller, "A")
        XCTAssertEqual(trades[0].instrument, "AUDUSD")
        XCTAssertEqual(trades[0].quantity, 100)
        XCTAssertEqual(trades[0].price, 1.47)

        XCTAssertEqual(orderBook.asks[0].trader, "A")
        XCTAssertEqual(orderBook.asks[0].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.asks[0].quantity, -150)
        XCTAssertEqual(orderBook.asks[0].price, 1.47)
    }

    func testMatchBidMultiple()
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.insert(order: Order(fromString: "A:AUDUSD:100:1.47"))
        trades += orderBook.insert(order: Order(fromString: "B:AUDUSD:200:1.46"))
        trades += orderBook.insert(order: Order(fromString: "C:AUDUSD:-150:1.46"))

        XCTAssertEqual(orderBook.bids.count, 1)
        XCTAssertEqual(orderBook.asks.count, 0)
        XCTAssertEqual(trades.count, 2)

        XCTAssertEqual(trades[0].buyer, "A")
        XCTAssertEqual(trades[0].seller, "C")
        XCTAssertEqual(trades[0].instrument, "AUDUSD")
        XCTAssertEqual(trades[0].quantity, 100)
        XCTAssertEqual(trades[0].price, 1.47)

        XCTAssertEqual(trades[1].buyer, "B")
        XCTAssertEqual(trades[1].seller, "C")
        XCTAssertEqual(trades[1].instrument, "AUDUSD")
        XCTAssertEqual(trades[1].quantity, 50)
        XCTAssertEqual(trades[1].price, 1.46)

        XCTAssertEqual(orderBook.bids[0].trader, "B")
        XCTAssertEqual(orderBook.bids[0].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.bids[0].quantity, 150)
        XCTAssertEqual(orderBook.bids[0].price, 1.46)
    }

    func testMatchAskMultiple()
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.insert(order: Order(fromString: "A:AUDUSD:-100:1.47"))
        trades += orderBook.insert(order: Order(fromString: "B:AUDUSD:-200:1.48"))
        trades += orderBook.insert(order: Order(fromString: "C:AUDUSD:150:1.48"))

        XCTAssertEqual(orderBook.bids.count, 0)
        XCTAssertEqual(orderBook.asks.count, 1)
        XCTAssertEqual(trades.count, 2)

        XCTAssertEqual(trades[0].buyer, "C")
        XCTAssertEqual(trades[0].seller, "A")
        XCTAssertEqual(trades[0].instrument, "AUDUSD")
        XCTAssertEqual(trades[0].quantity, 100)
        XCTAssertEqual(trades[0].price, 1.47)

        XCTAssertEqual(trades[1].buyer, "C")
        XCTAssertEqual(trades[1].seller, "B")
        XCTAssertEqual(trades[1].instrument, "AUDUSD")
        XCTAssertEqual(trades[1].quantity, 50)
        XCTAssertEqual(trades[1].price, 1.48)

        XCTAssertEqual(orderBook.asks[0].trader, "B")
        XCTAssertEqual(orderBook.asks[0].instrument, "AUDUSD")
        XCTAssertEqual(orderBook.asks[0].quantity, 150)
        XCTAssertEqual(orderBook.asks[0].price, 1.48)
    }
    static var allTests =
    [
        ("testInit", testInit),
        ("testInsertBid", testInsertBid),
        ("testInsertAsk", testInsertAsk),
        ("testInsertBidsAtDifferentPrices", testInsertBidsAtDifferentPrices),
        ("testInsertAsksAtDifferentPrices", testInsertAsksAtDifferentPrices),
        ("testInsertBidsAtTheSamePrice", testInsertBidsAtTheSamePrice),
        ("testInsertAsksAtTheSamePrice", testInsertAsksAtTheSamePrice),
        ("testMatchNothing", testMatchNothing),
        ("testMatchBidExactly", testMatchBidExactly),
        ("testMatchAskExactly", testMatchAskExactly),
        ("testMatchBidPartial", testMatchBidPartial),
        ("testMatchAskPartial", testMatchAskPartial),
        ("testMatchBidMultiple", testMatchBidMultiple),
        ("testMatchAskMultiple", testMatchAskMultiple),
    ]
}

