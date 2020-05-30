/*
 * unit tests for OrderBook class
 * 
 * Uses Unity as the test framework (via git subtree), see
 *  https://github.com/ThrowTheSwitch/Unity.git
 */
#define UNITY_INCLUDE_DOUBLE
#include "unittest/unity.h"

#include "order_book.h"


void setUp(void) {}

void tearDown(void) {}

void test_create(void) {
    OrderBook ob;
    
    TEST_ASSERT_EQUAL_PTR(&ob, OrderBook_init(&ob, "AUDUSD"));
    
    TEST_ASSERT_EQUAL_INT(0, ob.buys.size);
    TEST_ASSERT_EQUAL_INT(0, ob.sells.size);
    TEST_ASSERT_EQUAL_STRING("AUDUSD", ob.instrument);
}

void test_append_buy(void) {
    OrderBook ob;
    OrderBook *op;
    
    TEST_ASSERT_EQUAL_PTR(&ob, OrderBook_init(&ob, "AUDUSD"));
    op = OrderBook_append(&ob, "A", 300, 3.45);
    TEST_ASSERT_NOT_NULL(op);
    TEST_ASSERT_EQUAL_INT(1, ob.buys.size);
    TEST_ASSERT_EQUAL_INT(300, ob.buys.orders[0].qty);
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_append_buy);
    return UNITY_END();
}
