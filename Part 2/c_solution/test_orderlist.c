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
#include "order_book.h"

void setUp(void) {}

void tearDown(void) {}


void test_create(void) {
    OrderList ol;
    
    TEST_ASSERT_EQUAL_PTR(&ol, OrderList_init(&ol));
    
    TEST_ASSERT_EQUAL_INT(0, ol.size);
    TEST_ASSERT_GREATER_THAN_INT(0, ol.capacity);
}

void test_append(void) {
    OrderList ol;

    OrderList_init(&ol);

    TEST_ASSERT_NOT_NULL(OrderList_append(&ol, "A", 200, 1.32));
    TEST_ASSERT_EQUAL_INT(1, ol.size);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(1, ol.capacity);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_DOUBLE(1.32, ol.orders[0].price);
}

void test_append_two(void) {
    OrderList ol;

    OrderList_init(&ol);
    OrderList_append(&ol, "A", 200, 1.32);
    OrderList_append(&ol, "B", 100, 1.32);

    TEST_ASSERT_EQUAL_INT(2, ol.size);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
    TEST_ASSERT_EQUAL_INT(100, ol.orders[1].qty);
    TEST_ASSERT_EQUAL_STRING("B", ol.orders[1].id);
}


void test_find_lowest_even(void) {
    OrderList ol;

    int idx;

    OrderList_init(&ol);
    OrderList_append(&ol, "A", 200, 1.31);
    OrderList_append(&ol, "B", 100, 1.31);

    TEST_ASSERT_EQUAL_INT(2, ol.size);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
    
    idx = OrderList_remove_find_lowest(&ol, 0);

    TEST_ASSERT_EQUAL_INT(0, idx);

    TEST_ASSERT_EQUAL_INT(1, ol.size);
    TEST_ASSERT_EQUAL_INT(100, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("B", ol.orders[0].id);
}

void test_find_lowest(void) {
    OrderList ol;
    int idx;

    OrderList_init(&ol);
    OrderList_append(&ol, "A", 200, 1.31);
    OrderList_append(&ol, "B", 100, 1.30);
    OrderList_append(&ol, "C", 300, 1.30);

    TEST_ASSERT_EQUAL_INT(3, ol.size);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
    
    idx = OrderList_remove_find_lowest(&ol, 1);
    TEST_ASSERT_EQUAL_INT(1, idx);

    TEST_ASSERT_EQUAL_INT(2, ol.size);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);

    TEST_ASSERT_EQUAL_INT(300, ol.orders[idx].qty);
    TEST_ASSERT_EQUAL_STRING("C", ol.orders[idx].id);
}

void test_find_highest(void) {
    OrderList ol;
    int idx;

    OrderList_init(&ol);
    OrderList_append(&ol, "A", 200, 1.31);
    OrderList_append(&ol, "C", 300, 1.32);
    OrderList_append(&ol, "B", 100, 1.32);

    TEST_ASSERT_EQUAL_INT(3, ol.size);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
    
    idx = OrderList_remove_find_highest(&ol, 1);

    TEST_ASSERT_EQUAL_INT(2, ol.size);
    TEST_ASSERT_EQUAL_INT(1, idx);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
    TEST_ASSERT_EQUAL_INT(100, ol.orders[idx].qty);
    TEST_ASSERT_EQUAL_STRING("B", ol.orders[idx].id);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_append);
    RUN_TEST(test_append_two);

    RUN_TEST(test_find_highest);
    RUN_TEST(test_find_lowest);

    return UNITY_END();
}

