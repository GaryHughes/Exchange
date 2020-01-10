import XCTest
@testable import ExchangeLib

class OrderBookTests : XCTestCase
{
    func testInit()
    {
        let orderBook = OrderBook()

        XCTAssertEqual(orderBook.buyOrders.count, 0)
        XCTAssertEqual(orderBook.buyOrders.count, 0)
    }

    func testInsertBid() throws
    {
        let orderBook = OrderBook()

        let trades = orderBook.execute(order: try Order(fromString: "A:AUDUSD:100:1.47"))

        XCTAssertEqual(orderBook.buyOrders.count, 1)
        XCTAssertEqual(orderBook.sellOrders.count, 0)
        XCTAssertEqual(trades.count, 0)

        let one = orderBook.buyOrders.dequeue()!
        XCTAssertEqual(one.participant, "A")
        XCTAssertEqual(one.instrument, "AUDUSD")
        XCTAssertEqual(one.quantity, 100)
        XCTAssertEqual(one.price, 1.47)
    }

    func testInsertAsk() throws
    {
        let orderBook = OrderBook()

        let trades = orderBook.execute(order: try Order(fromString: "B:AUDUSD:-100:1.47"))

        XCTAssertEqual(orderBook.buyOrders.count, 0)
        XCTAssertEqual(orderBook.sellOrders.count, 1)
        XCTAssertEqual(trades.count, 0)

        let two = orderBook.sellOrders.dequeue()!
        XCTAssertEqual(two.participant, "B")
        XCTAssertEqual(two.instrument, "AUDUSD")
        XCTAssertEqual(two.quantity, -100)
        XCTAssertEqual(two.price, 1.47)
    }
   
    func testInsertBidsAtDifferentPrices() throws
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.execute(order: try Order(fromString: "A:AUDUSD:100:1.47"))
        trades += orderBook.execute(order: try Order(fromString: "B:AUDUSD:200:1.48"))
        trades += orderBook.execute(order: try Order(fromString: "C:AUDUSD:300:1.49"))

        XCTAssertEqual(orderBook.buyOrders.count, 3)
        XCTAssertEqual(orderBook.sellOrders.count, 0)
        XCTAssertEqual(trades.count, 0)

        let one = orderBook.buyOrders.dequeue()!
        XCTAssertEqual(one.participant, "C")
        XCTAssertEqual(one.instrument, "AUDUSD")
        XCTAssertEqual(one.quantity, 300)
        XCTAssertEqual(one.price, 1.49)

        let two = orderBook.buyOrders.dequeue()!
        XCTAssertEqual(two.participant, "B")
        XCTAssertEqual(two.instrument, "AUDUSD")
        XCTAssertEqual(two.quantity, 200)
        XCTAssertEqual(two.price, 1.48)

        let three = orderBook.buyOrders.dequeue()!
        XCTAssertEqual(three.participant, "A")
        XCTAssertEqual(three.instrument, "AUDUSD")
        XCTAssertEqual(three.quantity, 100)
        XCTAssertEqual(three.price, 1.47)
    }

    func testInsertAsksAtDifferentPrices() throws
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.execute(order: try Order(fromString: "A:AUDUSD:-100:1.49"))
        trades += orderBook.execute(order: try Order(fromString: "B:AUDUSD:-200:1.48"))
        trades += orderBook.execute(order: try Order(fromString: "C:AUDUSD:-300:1.47"))

        XCTAssertEqual(orderBook.buyOrders.count, 0)
        XCTAssertEqual(orderBook.sellOrders.count, 3)
        XCTAssertEqual(trades.count, 0)

        let one = orderBook.sellOrders.dequeue()!
        XCTAssertEqual(one.participant, "C")
        XCTAssertEqual(one.instrument, "AUDUSD")
        XCTAssertEqual(one.quantity, -300)
        XCTAssertEqual(one.price, 1.47)

        let two = orderBook.sellOrders.dequeue()!
        XCTAssertEqual(two.participant, "B")
        XCTAssertEqual(two.instrument, "AUDUSD")
        XCTAssertEqual(two.quantity, -200)
        XCTAssertEqual(two.price, 1.48)

        let three = orderBook.sellOrders.dequeue()!
        XCTAssertEqual(three.participant, "A")
        XCTAssertEqual(three.instrument, "AUDUSD")
        XCTAssertEqual(three.quantity, -100)
        XCTAssertEqual(three.price, 1.49)
    }

    func testInsertBidsAtTheSamePrice() throws
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.execute(order: try Order(fromString: "A:AUDUSD:100:1.47"))
        trades += orderBook.execute(order: try Order(fromString: "B:AUDUSD:200:1.47"))
        trades += orderBook.execute(order: try Order(fromString: "C:AUDUSD:300:1.47"))

        XCTAssertEqual(orderBook.buyOrders.count, 3)
        XCTAssertEqual(orderBook.sellOrders.count, 0)
        XCTAssertEqual(trades.count, 0)

        let one = orderBook.buyOrders.dequeue()!
        XCTAssertEqual(one.participant, "A")
        XCTAssertEqual(one.instrument, "AUDUSD")
        XCTAssertEqual(one.quantity, 100)
        XCTAssertEqual(one.price, 1.47)

        let two = orderBook.buyOrders.dequeue()!
        XCTAssertEqual(two.participant, "B")
        XCTAssertEqual(two.instrument, "AUDUSD")
        XCTAssertEqual(two.quantity, 200)
        XCTAssertEqual(two.price, 1.47)

        let three = orderBook.buyOrders.dequeue()!
        XCTAssertEqual(three.participant, "C")
        XCTAssertEqual(three.instrument, "AUDUSD")
        XCTAssertEqual(three.quantity, 300)
        XCTAssertEqual(three.price, 1.47)
    }

    func testInsertAsksAtTheSamePrice() throws
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.execute(order: try Order(fromString: "A:AUDUSD:-100:1.47"))
        trades += orderBook.execute(order: try Order(fromString: "B:AUDUSD:-200:1.47"))
        trades += orderBook.execute(order: try Order(fromString: "C:AUDUSD:-300:1.47"))

        XCTAssertEqual(orderBook.buyOrders.count, 0)
        XCTAssertEqual(orderBook.sellOrders.count, 3)
        XCTAssertEqual(trades.count, 0)

        let one = orderBook.sellOrders.dequeue()!
        XCTAssertEqual(one.participant, "A")
        XCTAssertEqual(one.instrument, "AUDUSD")
        XCTAssertEqual(one.quantity, -100)
        XCTAssertEqual(one.price, 1.47)

        let two = orderBook.sellOrders.dequeue()!
        XCTAssertEqual(two.participant, "B")
        XCTAssertEqual(two.instrument, "AUDUSD")
        XCTAssertEqual(two.quantity, -200)
        XCTAssertEqual(two.price, 1.47)

        let three = orderBook.sellOrders.dequeue()!
        XCTAssertEqual(three.participant, "C")
        XCTAssertEqual(three.instrument, "AUDUSD")
        XCTAssertEqual(three.quantity, -300)
        XCTAssertEqual(three.price, 1.47)
    }

    func testMatchNothing() throws
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.execute(order: try Order(fromString: "A:AUDUSD:100:1.47"))
        trades += orderBook.execute(order: try Order(fromString: "B:AUDUSD:-100:1.48"))

        XCTAssertEqual(orderBook.buyOrders.count, 1)
        XCTAssertEqual(orderBook.sellOrders.count, 1)
        XCTAssertEqual(trades.count, 0)

        let buy = orderBook.buyOrders.dequeue()!
        XCTAssertEqual(buy.participant, "A")
        XCTAssertEqual(buy.instrument, "AUDUSD")
        XCTAssertEqual(buy.quantity, 100)
        XCTAssertEqual(buy.price, 1.47)

        let sell = orderBook.sellOrders.dequeue()!
        XCTAssertEqual(sell.participant, "B")
        XCTAssertEqual(sell.instrument, "AUDUSD")
        XCTAssertEqual(sell.quantity, -100)
        XCTAssertEqual(sell.price, 1.48)
    }

    func testMatchBidExactly() throws
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.execute(order: try Order(fromString: "A:AUDUSD:100:1.47"))
        trades += orderBook.execute(order: try Order(fromString: "B:AUDUSD:-100:1.47"))

        XCTAssertEqual(orderBook.buyOrders.count, 0)
        XCTAssertEqual(orderBook.sellOrders.count, 0)
        XCTAssertEqual(trades.count, 1)

        XCTAssertEqual(trades[0].buyer, "A")
        XCTAssertEqual(trades[0].seller, "B")
        XCTAssertEqual(trades[0].instrument, "AUDUSD")
        XCTAssertEqual(trades[0].quantity, 100)
        XCTAssertEqual(trades[0].price, 1.47)
    }
    
    func testMatchAskExactly() throws
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.execute(order: try Order(fromString: "A:AUDUSD:-100:1.47"))
        trades += orderBook.execute(order: try Order(fromString: "B:AUDUSD:100:1.47"))

        XCTAssertEqual(orderBook.buyOrders.count, 0)
        XCTAssertEqual(orderBook.sellOrders.count, 0)
        XCTAssertEqual(trades.count, 1)

        XCTAssertEqual(trades[0].buyer, "B")
        XCTAssertEqual(trades[0].seller, "A")
        XCTAssertEqual(trades[0].instrument, "AUDUSD")
        XCTAssertEqual(trades[0].quantity, 100)
        XCTAssertEqual(trades[0].price, 1.47)
    }
    
    func testMatchBidPartial() throws
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.execute(order: try Order(fromString: "A:AUDUSD:250:1.47"))
        trades += orderBook.execute(order: try Order(fromString: "B:AUDUSD:-100:1.47"))

        XCTAssertEqual(orderBook.buyOrders.count, 1)
        XCTAssertEqual(orderBook.sellOrders.count, 0)
        XCTAssertEqual(trades.count, 1)

        XCTAssertEqual(trades[0].buyer, "A")
        XCTAssertEqual(trades[0].seller, "B")
        XCTAssertEqual(trades[0].instrument, "AUDUSD")
        XCTAssertEqual(trades[0].quantity, 100)
        XCTAssertEqual(trades[0].price, 1.47)

        let buy = orderBook.buyOrders.dequeue()!
        XCTAssertEqual(buy.participant, "A")
        XCTAssertEqual(buy.instrument, "AUDUSD")
        XCTAssertEqual(buy.remainingQuantity, 150)
        XCTAssertEqual(buy.price, 1.47)
    }

    func testMatchAskPartial() throws
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.execute(order: try Order(fromString: "A:AUDUSD:-250:1.47"))
        trades += orderBook.execute(order: try Order(fromString: "B:AUDUSD:100:1.47"))

        XCTAssertEqual(orderBook.buyOrders.count, 0)
        XCTAssertEqual(orderBook.sellOrders.count, 1)
        XCTAssertEqual(trades.count, 1)

        XCTAssertEqual(trades[0].buyer, "B")
        XCTAssertEqual(trades[0].seller, "A")
        XCTAssertEqual(trades[0].instrument, "AUDUSD")
        XCTAssertEqual(trades[0].quantity, 100)
        XCTAssertEqual(trades[0].price, 1.47)

        let sell = orderBook.sellOrders.dequeue()!
        XCTAssertEqual(sell.participant, "A")
        XCTAssertEqual(sell.instrument, "AUDUSD")
        XCTAssertEqual(sell.remainingQuantity, 150)
        XCTAssertEqual(sell.price, 1.47)
    }

    func testMatchBidMultiple() throws
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.execute(order: try Order(fromString: "A:AUDUSD:100:1.47"))
        trades += orderBook.execute(order: try Order(fromString: "B:AUDUSD:200:1.46"))
        trades += orderBook.execute(order: try Order(fromString: "C:AUDUSD:-150:1.46"))

        XCTAssertEqual(orderBook.buyOrders.count, 1)
        XCTAssertEqual(orderBook.sellOrders.count, 0)
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

        let buy = orderBook.buyOrders.dequeue()!
        XCTAssertEqual(buy.participant, "B")
        XCTAssertEqual(buy.instrument, "AUDUSD")
        XCTAssertEqual(buy.remainingQuantity, 150)
        XCTAssertEqual(buy.price, 1.46)
    }

    func testMatchAskMultiple() throws
    {
        let orderBook = OrderBook()

        var trades : [Trade] = []
        trades += orderBook.execute(order: try Order(fromString: "A:AUDUSD:-100:1.47"))
        trades += orderBook.execute(order: try Order(fromString: "B:AUDUSD:-200:1.48"))
        trades += orderBook.execute(order: try Order(fromString: "C:AUDUSD:150:1.48"))

        XCTAssertEqual(orderBook.buyOrders.count, 0)
        XCTAssertEqual(orderBook.sellOrders.count, 1)
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

        let sell = orderBook.sellOrders.dequeue()!
        XCTAssertEqual(sell.participant, "B")
        XCTAssertEqual(sell.instrument, "AUDUSD")
        XCTAssertEqual(sell.remainingQuantity, 150)
        XCTAssertEqual(sell.price, 1.48)
    }
    
}

