#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE order_tests
#include <boost/test/unit_test.hpp>
#include "order.h"

BOOST_AUTO_TEST_CASE( test_parse_missing_price )
{
	BOOST_REQUIRE_THROW(ae::order::parse("A:AUDUSD:100"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE( test_invalid_price )
{
	BOOST_REQUIRE_THROW(ae::order::parse("A:AUDUSD:100:blah"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE( test_zero_price_rejected )
{
	BOOST_REQUIRE_THROW(ae::order::parse("A:AUDUSD:100:0"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE( test_invalid_quantity )
{
	BOOST_REQUIRE_THROW(ae::order::parse("A:AUDUSD:xx:23.45"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE( test_zero_quantity )
{
	BOOST_REQUIRE_THROW(ae::order::parse("A:AUDUSD:0:23.45"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE( test_zero_price )
{
	BOOST_REQUIRE_THROW(ae::order::parse("A:AUDUSD:1000:0"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE( test_parse_buy )
{
	auto order = ae::order::parse("A:AUDUSD:100:1.47");
    BOOST_REQUIRE_EQUAL("A", order.participant());
    BOOST_REQUIRE_EQUAL("AUDUSD", order.instrument());
    BOOST_REQUIRE_EQUAL(100, order.quantity());
    BOOST_REQUIRE_EQUAL(1.47, order.price());
    BOOST_REQUIRE(order.is_buy());
}

BOOST_AUTO_TEST_CASE( test_parse_sell )
{
	auto order = ae::order::parse("A:AUDUSD:-100:1.47");
    BOOST_REQUIRE_EQUAL("A", order.participant());
    BOOST_REQUIRE_EQUAL("AUDUSD", order.instrument());
    BOOST_REQUIRE_EQUAL(100, order.quantity());
    BOOST_REQUIRE_EQUAL(1.47, order.price());
    BOOST_REQUIRE(!order.is_buy());
}

