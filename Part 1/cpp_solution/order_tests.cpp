#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE order_tests
#include <boost/test/unit_test.hpp>
#include "order.h"

BOOST_AUTO_TEST_CASE( test_parse_missing_price )
{
	std::istringstream is("A:AUDUSD:100");
	ae::order order;
	BOOST_REQUIRE_THROW(is >> order, std::runtime_error);
}

BOOST_AUTO_TEST_CASE( test_invalid_price )
{
	std::istringstream is("A:AUDUSD:100:blah");
	ae::order order;
	BOOST_REQUIRE_THROW(is >> order, std::runtime_error);
}

BOOST_AUTO_TEST_CASE( test_zero_price_rejected )
{
	std::istringstream is("A:AUDUSD:100:0");
	ae::order order;
	BOOST_REQUIRE_THROW(is >> order, std::runtime_error);
}

BOOST_AUTO_TEST_CASE( test_invalid_quantity )
{
	std::istringstream is("A:AUDUSD:xx:23.45");
	ae::order order;
	BOOST_REQUIRE_THROW(is >> order, std::runtime_error);
}

BOOST_AUTO_TEST_CASE( test_zero_quantity )
{
	std::istringstream is("A:AUDUSD:0:23.45");
	ae::order order;
	BOOST_REQUIRE_THROW(is >> order, std::runtime_error);
}

BOOST_AUTO_TEST_CASE( test_zero_price )
{
	std::istringstream is("A:AUDUSD:1000:0");
	ae::order order;
	BOOST_REQUIRE_THROW(is >> order, std::runtime_error);
}

BOOST_AUTO_TEST_CASE( test_parse_buy )
{
	std::istringstream is("A:AUDUSD:100:1.47");
	ae::order order;
	BOOST_REQUIRE(is >> order);
    BOOST_REQUIRE_EQUAL("A", order.participant());
    BOOST_REQUIRE_EQUAL("AUDUSD", order.instrument());
    BOOST_REQUIRE_EQUAL(100, order.quantity());
    BOOST_REQUIRE_EQUAL(1.47, order.price());
    BOOST_REQUIRE(order.is_buy());
}

BOOST_AUTO_TEST_CASE( test_parse_sell )
{
	std::istringstream is("A:AUDUSD:-100:1.47");
	ae::order order;
	BOOST_REQUIRE(is >> order);
    BOOST_REQUIRE_EQUAL("A", order.participant());
    BOOST_REQUIRE_EQUAL("AUDUSD", order.instrument());
    BOOST_REQUIRE_EQUAL(100, order.quantity());
    BOOST_REQUIRE_EQUAL(1.47, order.price());
    BOOST_REQUIRE(!order.is_buy());
}

