/*
 * unit tests for OrderList class
 * 
 * Uses Unity as the test framework (via git subtree), see
 *  https://github.com/ThrowTheSwitch/Unity.git
 */

#include <string.h>

#define UNITY_INCLUDE_DOUBLE
#include "unittest/unity.h"

#include "order_list.h"

void setUp(void) {}

void tearDown(void) {}


void test_create(void) {
    OrderList ol;
    
    TEST_ASSERT_EQUAL_PTR(&ol, OrderList_init(&ol));
    
    TEST_ASSERT_EQUAL_INT(0, ol.size);
    TEST_ASSERT_GREATER_THAN_INT(0, ol.capacity);

    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 0));
    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 1));
}

void test_push_min(void) {
    OrderList ol;

    OrderList_init(&ol);

    TEST_ASSERT_NOT_NULL(OrderList_push_min(&ol, "A", 200, 1.32));
    TEST_ASSERT_EQUAL_INT(1, ol.size);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(1, ol.capacity);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_DOUBLE(1.32, ol.orders[0].price);
    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 1));
}

void test_push_max(void) {
    OrderList ol;

    OrderList_init(&ol);

    TEST_ASSERT_NOT_NULL(OrderList_push_max(&ol, "A", 200, 1.32));
    TEST_ASSERT_EQUAL_INT(1, ol.size);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(1, ol.capacity);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_DOUBLE(1.32, ol.orders[0].price);
    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 0));
}

void test_push_min_two_equal(void) {
    OrderList ol;

    OrderList_init(&ol);
    OrderList_push_min(&ol, "A", 200, 1.32);
    OrderList_push_min(&ol, "B", 100, 1.32);

    TEST_ASSERT_EQUAL_INT(2, ol.size);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
    TEST_ASSERT_EQUAL_INT(100, ol.orders[1].qty);
    TEST_ASSERT_EQUAL_STRING("B", ol.orders[1].id);
    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 1));
}

void test_push_max_two_equal(void) {
    OrderList ol;

    OrderList_init(&ol);
    OrderList_push_max(&ol, "A", 200, 1.32);
    OrderList_push_max(&ol, "B", 100, 1.32);

    TEST_ASSERT_EQUAL_INT(2, ol.size);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
    TEST_ASSERT_EQUAL_INT(100, ol.orders[1].qty);
    TEST_ASSERT_EQUAL_STRING("B", ol.orders[1].id);
    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 0));
}

void test_min_5(void) {
    OrderList ol;

    OrderList_init(&ol);
    OrderList_push_min(&ol, "A", 100, 1.32);
    OrderList_push_min(&ol, "B", 200, 1.32);
    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 1));
    OrderList_push_min(&ol, "C", 300, 1.31);
    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 1));
    OrderList_push_min(&ol, "D", 400, 1.33);
    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 1));
    OrderList_push_min(&ol, "E", 500, 1.32);

    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 1));

    TEST_ASSERT_EQUAL_INT(5, ol.size);
    TEST_ASSERT_EQUAL_INT(300, ol.orders[0].qty);
    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 1));

    OrderList_pop_min(&ol);
    TEST_ASSERT_EQUAL_INT(4, ol.size);
    TEST_ASSERT_EQUAL_INT(100, ol.orders[0].qty);
    OrderList_pop_min(&ol);
    TEST_ASSERT_EQUAL_INT(3, ol.size);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    OrderList_pop_min(&ol);
    TEST_ASSERT_EQUAL_INT(2, ol.size);
    TEST_ASSERT_EQUAL_INT(500, ol.orders[0].qty);
    OrderList_pop_min(&ol);
    TEST_ASSERT_EQUAL_INT(1, ol.size);
    TEST_ASSERT_EQUAL_INT(400, ol.orders[0].qty);
    OrderList_pop_min(&ol);
    TEST_ASSERT_EQUAL_INT(0, ol.size);
}

void test_max_5(void) {
    OrderList ol;

    OrderList_init(&ol);
    OrderList_push_max(&ol, "A", 100, 1.32);
    OrderList_push_max(&ol, "B", 200, 1.32);
    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 0));
    OrderList_push_max(&ol, "C", 300, 1.31);
    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 0));
    OrderList_push_max(&ol, "D", 400, 1.33);
    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 0));
    OrderList_push_max(&ol, "E", 500, 1.32);

    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 0));

    TEST_ASSERT_EQUAL_INT(5, ol.size);
    TEST_ASSERT_EQUAL_INT(400, ol.orders[0].qty);
    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 0));

    OrderList_pop_max(&ol);
    TEST_ASSERT_EQUAL_INT(4, ol.size);
    TEST_ASSERT_EQUAL_INT(100, ol.orders[0].qty);
    OrderList_pop_max(&ol);
    TEST_ASSERT_EQUAL_INT(3, ol.size);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    OrderList_pop_max(&ol);
    TEST_ASSERT_EQUAL_INT(2, ol.size);
    TEST_ASSERT_EQUAL_INT(500, ol.orders[0].qty);
    OrderList_pop_max(&ol);
    TEST_ASSERT_EQUAL_INT(1, ol.size);
    TEST_ASSERT_EQUAL_INT(300, ol.orders[0].qty);
    OrderList_pop_max(&ol);
    TEST_ASSERT_EQUAL_INT(0, ol.size);
}

/* A test long enough thst pop() needs to recurse */
void test_max_9_order(void) {
    OrderList ol;

    OrderList_init(&ol);
    OrderList_push_max(&ol, "A", 100, 1.31);
    OrderList_push_max(&ol, "B", 200, 1.32);
    OrderList_push_max(&ol, "C", 300, 1.33);
    OrderList_push_max(&ol, "D", 400, 1.34);
    OrderList_push_max(&ol, "E", 500, 1.35);
    OrderList_push_max(&ol, "F", 600, 1.34);
    OrderList_push_max(&ol, "G", 700, 1.33);
    OrderList_push_max(&ol, "H", 800, 1.32);
    OrderList_push_max(&ol, "I", 900, 1.31);

    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 0));

    TEST_ASSERT_EQUAL_INT(9, ol.size);
    TEST_ASSERT_EQUAL_INT(500, ol.orders[0].qty);
    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 0));

    OrderList_pop_max(&ol);
    TEST_ASSERT_EQUAL_INT(8, ol.size);
    TEST_ASSERT_EQUAL_INT(400, ol.orders[0].qty);
    OrderList_pop_max(&ol);
    TEST_ASSERT_EQUAL_INT(7, ol.size);
    TEST_ASSERT_EQUAL_INT(600, ol.orders[0].qty);
    OrderList_pop_max(&ol);
    TEST_ASSERT_EQUAL_INT(6, ol.size);
    TEST_ASSERT_EQUAL_INT(300, ol.orders[0].qty);
    OrderList_pop_max(&ol);
    TEST_ASSERT_EQUAL_INT(5, ol.size);
    TEST_ASSERT_EQUAL_INT(700, ol.orders[0].qty);
    OrderList_pop_max(&ol);
    TEST_ASSERT_EQUAL_INT(4, ol.size);
}

/* A test long enough thst pop() needs to recurse */
void test_min_9_order(void) {
    OrderList ol;

    OrderList_init(&ol);
    OrderList_push_min(&ol, "A", 100, 1.31);
    OrderList_push_min(&ol, "B", 200, 1.32);
    OrderList_push_min(&ol, "C", 300, 1.33);
    OrderList_push_min(&ol, "D", 400, 1.34);
    OrderList_push_min(&ol, "E", 500, 1.35);
    OrderList_push_min(&ol, "F", 600, 1.35);
    OrderList_push_min(&ol, "G", 700, 1.35);
    OrderList_push_min(&ol, "H", 800, 1.35);
    OrderList_push_min(&ol, "I", 900, 1.35);

    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 1));

    TEST_ASSERT_EQUAL_INT(9, ol.size);
    TEST_ASSERT_EQUAL_INT(100, ol.orders[0].qty);
    TEST_ASSERT_TRUE(OrderList_check_heap(&ol, 1));

    OrderList_pop_min(&ol);
    TEST_ASSERT_EQUAL_INT(8, ol.size);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    OrderList_pop_min(&ol);
    TEST_ASSERT_EQUAL_INT(7, ol.size);
    TEST_ASSERT_EQUAL_INT(300, ol.orders[0].qty);
    OrderList_pop_min(&ol);
    TEST_ASSERT_EQUAL_INT(6, ol.size);
    TEST_ASSERT_EQUAL_INT(400, ol.orders[0].qty);
    OrderList_pop_min(&ol);
    TEST_ASSERT_EQUAL_INT(5, ol.size);
    TEST_ASSERT_EQUAL_INT(500, ol.orders[0].qty);
    OrderList_pop_min(&ol);
    TEST_ASSERT_EQUAL_INT(4, ol.size);
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_push_min);
    RUN_TEST(test_push_max);
    RUN_TEST(test_push_min_two_equal);
    RUN_TEST(test_push_max_two_equal);
    RUN_TEST(test_min_5);
    RUN_TEST(test_max_5);
    RUN_TEST(test_min_9_order);
    RUN_TEST(test_max_9_order);

    return UNITY_END();
}

