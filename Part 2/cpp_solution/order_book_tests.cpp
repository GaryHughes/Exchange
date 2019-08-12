#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE order_book_tests
#include <boost/test/unit_test.hpp>
#include "order_book.h"

BOOST_AUTO_TEST_CASE( test_buy_ordering )
{
	ae::order_book book("AUDUSD");

    book.insert(ae::order("A", "AUDUSD", 100, 10));
    book.insert(ae::order("A", "AUDUSD", 100, 7));
    book.insert(ae::order("A", "AUDUSD", 100, 11));
    book.insert(ae::order("A", "AUDUSD", 100, 9));

    BOOST_REQUIRE_EQUAL(4, book.buy_orders().size());
    BOOST_REQUIRE_EQUAL(0, book.sell_orders().size());

    BOOST_REQUIRE_EQUAL(11, book.buy_orders().top().price()); 
    book.buy_orders().pop();

    BOOST_REQUIRE_EQUAL(10, book.buy_orders().top().price()); 
    book.buy_orders().pop();

    BOOST_REQUIRE_EQUAL(9, book.buy_orders().top().price()); 
    book.buy_orders().pop();

    BOOST_REQUIRE_EQUAL(7, book.buy_orders().top().price()); 
    book.buy_orders().pop();
}

BOOST_AUTO_TEST_CASE( test_buy_ordering_same_price )
{
	ae::order_book book("AUDUSD");

    book.insert(ae::order("A", "AUDUSD", 100, 10));
    book.insert(ae::order("A", "AUDUSD", 101, 10));
    book.insert(ae::order("A", "AUDUSD", 102, 10));
    book.insert(ae::order("A", "AUDUSD", 103, 10));

    BOOST_REQUIRE_EQUAL(4, book.buy_orders().size());
    BOOST_REQUIRE_EQUAL(0, book.sell_orders().size());

    BOOST_REQUIRE_EQUAL(100, book.buy_orders().top().quantity());
    book.buy_orders().pop();

    BOOST_REQUIRE_EQUAL(101, book.buy_orders().top().quantity());
    book.buy_orders().pop();

    BOOST_REQUIRE_EQUAL(102, book.buy_orders().top().quantity());
    book.buy_orders().pop();

    BOOST_REQUIRE_EQUAL(103, book.buy_orders().top().quantity());
    book.buy_orders().pop();
}

BOOST_AUTO_TEST_CASE( test_sell_ordering )
{
	ae::order_book book("AUDUSD");

    book.insert(ae::order("A", "AUDUSD", -100, 10));
    book.insert(ae::order("A", "AUDUSD", -100, 7));
    book.insert(ae::order("A", "AUDUSD", -100, 11));
    book.insert(ae::order("A", "AUDUSD", -100, 9));

    BOOST_REQUIRE_EQUAL(0, book.buy_orders().size());
    BOOST_REQUIRE_EQUAL(4, book.sell_orders().size());

    BOOST_REQUIRE_EQUAL(7, book.sell_orders().top().price());
    book.sell_orders().pop();    

    BOOST_REQUIRE_EQUAL(9, book.sell_orders().top().price());
    book.sell_orders().pop();    

    BOOST_REQUIRE_EQUAL(10, book.sell_orders().top().price());
    book.sell_orders().pop();    

    BOOST_REQUIRE_EQUAL(11, book.sell_orders().top().price());
    book.sell_orders().pop();    
}

BOOST_AUTO_TEST_CASE( test_sell_ordering_same_price )
{
	ae::order_book book("AUDUSD");

    book.insert(ae::order("A", "AUDUSD", -100, 10));
    book.insert(ae::order("A", "AUDUSD", -101, 10));
    book.insert(ae::order("A", "AUDUSD", -102, 10));
    book.insert(ae::order("A", "AUDUSD", -103, 10));

    BOOST_REQUIRE_EQUAL(0, book.buy_orders().size());
    BOOST_REQUIRE_EQUAL(4, book.sell_orders().size());

    BOOST_REQUIRE_EQUAL(100, book.sell_orders().top().quantity());
    book.sell_orders().pop();

    BOOST_REQUIRE_EQUAL(101, book.sell_orders().top().quantity());
    book.sell_orders().pop();

    BOOST_REQUIRE_EQUAL(102, book.sell_orders().top().quantity());
    book.sell_orders().pop();

    BOOST_REQUIRE_EQUAL(103, book.sell_orders().top().quantity());
    book.sell_orders().pop();
}

BOOST_AUTO_TEST_CASE( test_first_price_entered_selected )
{
	ae::order_book book("AUDUSD");

	book.insert(ae::order("B", "EURUSD", -100, 1.11));
	book.insert(ae::order("F", "EURUSD", -50, 1.1));
	book.insert(ae::order("D", "EURUSD", 100, 1.11));

	ae::trade_collection trades;

	book.match(trades);

	BOOST_REQUIRE_EQUAL(2, trades.size());

	auto trade = trades[0];

	BOOST_REQUIRE_EQUAL(1.1, trade.match_price());
	BOOST_REQUIRE_EQUAL(50, trade.match_quantity());

	trade = trades[1];

	BOOST_REQUIRE_EQUAL(1.11, trade.match_price());
	BOOST_REQUIRE_EQUAL(50, trade.match_quantity());
}
