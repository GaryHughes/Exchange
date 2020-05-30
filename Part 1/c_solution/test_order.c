/*
 * unit tests for Order class
 * 
 * Uses Unity as the test framework (via git subtree), see
 *  https://github.com/ThrowTheSwitch/Unity.git
 */
#define UNITY_INCLUDE_DOUBLE
#include "unittest/unity.h"

#include "order.h"


void setUp(void) {}

void tearDown(void) {}

void test_create_buy(void) {
    Order o;
    TEST_ASSERT_EQUAL_PTR(&o, Order_create(&o, "A:AUDUSD:100:1.47"));
    TEST_ASSERT_EQUAL_STRING("A", o.id);
    TEST_ASSERT_EQUAL_STRING("AUDUSD", o.instrument);
    TEST_ASSERT_EQUAL_INT(100, o.qty);
    TEST_ASSERT_EQUAL_DOUBLE(1.47, o.price);
    TEST_ASSERT_EQUAL_CHAR('B', o.side);
    TEST_ASSERT_GREATER_THAN_INT(0, o.generation);
}

void test_create_sell(void) {
    Order o;
    TEST_ASSERT_EQUAL_PTR(&o, Order_create(&o, "A:AUDUSD:-100:1.47\n"));
    TEST_ASSERT_EQUAL_STRING("A", o.id);
    TEST_ASSERT_EQUAL_STRING("AUDUSD", o.instrument);
    TEST_ASSERT_EQUAL_INT(100, o.qty);
    TEST_ASSERT_EQUAL_DOUBLE(1.47, o.price);
    TEST_ASSERT_EQUAL_CHAR('S', o.side);
    TEST_ASSERT_GREATER_THAN_INT(0, o.generation);
}

void test_create_failed(void) {
    Order o;
    TEST_ASSERT_NULL(Order_create(&o, "invalid"));
    TEST_ASSERT_NULL(Order_create(&o, ""));
}

void test_create_failed_too_long(void) {
    Order o;
    TEST_ASSERT_NULL(Order_create(&o, "A23456789:AUDUSD:100:1.46\n"));
    TEST_ASSERT_NULL(Order_create(&o, "A:AUDUSDextraextra:100:1.46\n"));
}

void test_generation(void) {
    Order o1, o2;
    TEST_ASSERT_EQUAL_PTR(&o1, Order_create(&o1, "A:AUDUSD:-100:1.47\n"));
    TEST_ASSERT_EQUAL_PTR(&o2, Order_create(&o2, "A:AUDUSD:-100:1.47\n"));
    TEST_ASSERT_TRUE(o2.generation > o1.generation);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_buy);
    RUN_TEST(test_create_sell);
    RUN_TEST(test_create_failed);
    RUN_TEST(test_create_failed_too_long);
    RUN_TEST(test_generation);
    return UNITY_END();
}
