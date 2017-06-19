import XCTest
@testable import ExchangeLibTests

XCTMain([
    testCase(OrderTests.allTests),
    testCase(TradeTests.allTests),
    testCase(OrderBookTests.allTests),
    testCase(ExchangeTests.allTests),
])

