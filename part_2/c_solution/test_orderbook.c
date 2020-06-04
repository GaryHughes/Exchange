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

void test_append_buy_higher(void) {
    OrderBook ob;
    OrderBook *op;
    
    TEST_ASSERT_EQUAL_PTR(&ob, OrderBook_init(&ob, "AUDUSD"));
    op = OrderBook_append(&ob, "A", 100, 3.45);
    op = OrderBook_append(&ob, "B", 200, 3.46);
    TEST_ASSERT_NOT_NULL(op);
    TEST_ASSERT_EQUAL_INT(2, ob.buys.size);
    TEST_ASSERT_EQUAL_INT(200, ob.buys.orders[0].qty);
}

void test_append_buy_same(void) {
    OrderBook ob;
    OrderBook *op;
    
    TEST_ASSERT_EQUAL_PTR(&ob, OrderBook_init(&ob, "AUDUSD"));
    op = OrderBook_append(&ob, "A", 100, 3.45);
    op = OrderBook_append(&ob, "B", 200, 3.45);
    TEST_ASSERT_NOT_NULL(op);
    TEST_ASSERT_EQUAL_INT(2, ob.buys.size);
    TEST_ASSERT_EQUAL_INT(100, ob.buys.orders[0].qty);
}

void test_append_sell(void) {
    OrderBook ob;
    OrderBook *op;
    
    TEST_ASSERT_EQUAL_PTR(&ob, OrderBook_init(&ob, "AUDUSD"));
    op = OrderBook_append(&ob, "A", -300, 3.45);
    TEST_ASSERT_NOT_NULL(op);
    TEST_ASSERT_EQUAL_INT(1, ob.sells.size);
    TEST_ASSERT_EQUAL_INT(300, ob.sells.orders[0].qty);
}


void test_append_sell_lower(void) {
    OrderBook ob;
    OrderBook *op;
    
    TEST_ASSERT_EQUAL_PTR(&ob, OrderBook_init(&ob, "AUDUSD"));
    op = OrderBook_append(&ob, "A", -100, 3.45);
    op = OrderBook_append(&ob, "B", -200, 3.44);
    TEST_ASSERT_NOT_NULL(op);
    TEST_ASSERT_EQUAL_INT(2, ob.sells.size);
    TEST_ASSERT_EQUAL_INT(200, ob.sells.orders[0].qty);
}

void test_append_sell_same(void) {
    OrderBook ob;
    OrderBook *op;
    
    TEST_ASSERT_EQUAL_PTR(&ob, OrderBook_init(&ob, "AUDUSD"));
    op = OrderBook_append(&ob, "A", -100, 3.45);
    op = OrderBook_append(&ob, "B", -200, 3.45);
    TEST_ASSERT_NOT_NULL(op);
    TEST_ASSERT_EQUAL_INT(2, ob.sells.size);
    TEST_ASSERT_EQUAL_INT(100, ob.sells.orders[0].qty);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_append_buy);
    RUN_TEST(test_append_buy_higher);
    RUN_TEST(test_append_buy_same);
    RUN_TEST(test_append_sell);
    RUN_TEST(test_append_sell_lower);
    RUN_TEST(test_append_sell_same);
    return UNITY_END();
}
