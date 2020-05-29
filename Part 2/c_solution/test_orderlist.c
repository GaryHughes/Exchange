/*
 * unit tests for OrderList class
 * 
 * Uses Unity as the test framework (via git subtree), see
 *  https://github.com/ThrowTheSwitch/Unity.git
 */
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
    Order o;
    OrderList_init(&ol);

    Order_init(&o, "A", 200, 1.32);

    TEST_ASSERT_NOT_NULL(OrderList_append(&ol, &o));
    TEST_ASSERT_EQUAL_INT(1, ol.size);
    TEST_ASSERT_GREATER_OR_EQUAL_INT(1, ol.capacity);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_DOUBLE(1.32, ol.orders[0].price);
    TEST_ASSERT_GREATER_THAN_INT(0, ol.orders[0].generation);
}

void test_append_two(void) {
    OrderList ol;
    Order o;
    OrderList_init(&ol);
    OrderList_append(&ol, Order_init(&o, "A", 200, 1.32));
    OrderList_append(&ol, Order_init(&o, "B", 100, 1.32));

    TEST_ASSERT_EQUAL_INT(2, ol.size);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
    TEST_ASSERT_EQUAL_INT(100, ol.orders[1].qty);
    TEST_ASSERT_EQUAL_STRING("B", ol.orders[1].id);
}

void test_sort_inorder(void) {
    OrderList ol;
    Order o;

    OrderList_init(&ol);
    OrderList_append(&ol, Order_init(&o, "A", 200, 1.32));
    OrderList_append(&ol, Order_init(&o, "B", 100, 1.31));

    OrderList_sort_descending(&ol);

    TEST_ASSERT_EQUAL_INT(2, ol.size);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
    TEST_ASSERT_EQUAL_INT(100, ol.orders[1].qty);
    TEST_ASSERT_EQUAL_STRING("B", ol.orders[1].id);

    /* Force a re-sort */
    ol.last_sorted_size = 0;
    OrderList_sort_ascending(&ol);

    TEST_ASSERT_EQUAL_INT(2, ol.size);
    TEST_ASSERT_EQUAL_INT(100, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("B", ol.orders[0].id);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[1].qty);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[1].id);
}

void test_sort_reversed(void) {
    OrderList ol;
    Order o;

    OrderList_init(&ol);
    OrderList_append(&ol, Order_init(&o, "A", 200, 1.32));
    OrderList_append(&ol, Order_init(&o, "B", 100, 1.31));

    OrderList_sort_ascending(&ol);

    TEST_ASSERT_EQUAL_INT(2, ol.size);
    TEST_ASSERT_EQUAL_INT(100, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("B", ol.orders[0].id);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[1].qty);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[1].id);

    /* Force a re-sort */
    ol.last_sorted_size = 0;
    OrderList_sort_descending(&ol);

    TEST_ASSERT_EQUAL_INT(2, ol.size);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
    TEST_ASSERT_EQUAL_INT(100, ol.orders[1].qty);
    TEST_ASSERT_EQUAL_STRING("B", ol.orders[1].id);
}


void test_sort_stable(void) {
    OrderList ol;
    Order o;

    OrderList_init(&ol);
    OrderList_append(&ol, Order_init(&o, "A", 200, 1.31));
    OrderList_append(&ol, Order_init(&o, "B", 100, 1.31));

    OrderList_sort_ascending(&ol);

    TEST_ASSERT_EQUAL_INT(2, ol.size);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
    TEST_ASSERT_EQUAL_INT(100, ol.orders[1].qty);

    OrderList_sort_descending(&ol);

    TEST_ASSERT_EQUAL_INT(2, ol.size);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
    TEST_ASSERT_EQUAL_INT(100, ol.orders[1].qty);
    TEST_ASSERT_EQUAL_STRING("B", ol.orders[1].id);
}

void test_remove_first(void) {
    OrderList ol;
    Order o;

    OrderList_init(&ol);
    OrderList_append(&ol, Order_init(&o, "A", 200, 1.31));
    OrderList_append(&ol, Order_init(&o, "B", 100, 1.31));

    TEST_ASSERT_EQUAL_INT(2, ol.size);
    TEST_ASSERT_EQUAL_INT(200, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("A", ol.orders[0].id);
   
    OrderList_remove_first(&ol);

    TEST_ASSERT_EQUAL_INT(1, ol.size);
    TEST_ASSERT_EQUAL_INT(100, ol.orders[0].qty);
    TEST_ASSERT_EQUAL_STRING("B", ol.orders[0].id);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_append);
    RUN_TEST(test_append_two);

    RUN_TEST(test_sort_inorder);
    RUN_TEST(test_sort_reversed);
    RUN_TEST(test_sort_stable);

    RUN_TEST(test_remove_first);

    return UNITY_END();
}
