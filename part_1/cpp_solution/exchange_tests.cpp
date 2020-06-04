#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE exchange_tests
#include <boost/test/unit_test.hpp>
#include "exchange.h"

BOOST_AUTO_TEST_CASE( test_match_with_different_participants )
{
	ae::exchange exchange;

	auto trades = exchange.execute(ae::order("A", "AUDUSD", 100, 10));

	BOOST_REQUIRE_EQUAL(0, trades.size());

	trades = exchange.execute(ae::order("B", "AUDUSD", -100, 10));

	BOOST_REQUIRE_EQUAL(1, trades.size());

	auto trade = trades[0];

	BOOST_REQUIRE_EQUAL("A", trade.buyer());
	BOOST_REQUIRE_EQUAL("B", trade.seller());
	BOOST_REQUIRE_EQUAL(100, trade.match_quantity());
	BOOST_REQUIRE_EQUAL(10, trade.match_price());
}

BOOST_AUTO_TEST_CASE( test_match_with_same_participant )
{
	ae::exchange exchange;

	auto trades = exchange.execute(ae::order("A", "AUDUSD", 100, 10));

	BOOST_REQUIRE_EQUAL(0, trades.size());

	trades = exchange.execute(ae::order("A", "AUDUSD", -100, 10));

	BOOST_REQUIRE_EQUAL(1, trades.size());

	auto trade = trades[0];

	BOOST_REQUIRE_EQUAL("A", trade.buyer());
	BOOST_REQUIRE_EQUAL("A", trade.seller());
	BOOST_REQUIRE_EQUAL(100, trade.match_quantity());
	BOOST_REQUIRE_EQUAL(10, trade.match_price());
}

BOOST_AUTO_TEST_CASE( test_different_instruments_do_not_match )
{
	ae::exchange exchange;

	auto trades = exchange.execute(ae::order("A", "AUDUSD", 100, 10));

	BOOST_REQUIRE_EQUAL(0, trades.size());

	trades = exchange.execute(ae::order("B", "EURGBP", -100, 10));

	BOOST_REQUIRE_EQUAL(0, trades.size());
}

BOOST_AUTO_TEST_CASE( test_price_from_first_order_selected )
{
	ae::exchange exchange;

	auto trades = exchange.execute(ae::order("A", "AUDUSD", 100, 10));

	BOOST_REQUIRE_EQUAL(0, trades.size());

	trades = exchange.execute(ae::order("B", "AUDUSD", -100, 9));

	BOOST_REQUIRE_EQUAL(1, trades.size());

	auto trade = trades[0];

	BOOST_REQUIRE_EQUAL("A", trade.buyer());
	BOOST_REQUIRE_EQUAL("B", trade.seller());
	BOOST_REQUIRE_EQUAL(100, trade.match_quantity());
	BOOST_REQUIRE_EQUAL(10, trade.match_price());
}

