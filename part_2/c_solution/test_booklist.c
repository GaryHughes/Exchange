/*
 * unit tests for BookList class
 * 
 * Uses Unity as the test framework (via git subtree), see
 *  https://github.com/ThrowTheSwitch/Unity.git
 */
#define UNITY_INCLUDE_DOUBLE
#include "unittest/unity.h"

#include "book_list.h"


void setUp(void) {}

void tearDown(void) {}

void test_create(void) {
    BookList bl;
    
    TEST_ASSERT_EQUAL_PTR(&bl, BookList_init(&bl));
    
    TEST_ASSERT_EQUAL_INT(0, bl.size);
}

void test_append(void) {
    BookList bl;
    OrderBook *ob;
    
    TEST_ASSERT_EQUAL_PTR(&bl, BookList_init(&bl));

    ob = BookList_find(&bl, "AUDUSD");
    TEST_ASSERT_NOT_NULL(ob);
    TEST_ASSERT_EQUAL_INT(1, bl.size);
    TEST_ASSERT_EQUAL_STRING("AUDUSD", ob->instrument);

    /* Find again should be idempotent */
    ob = BookList_find(&bl, "AUDUSD");
    TEST_ASSERT_NOT_NULL(ob);
    TEST_ASSERT_EQUAL_INT(1, bl.size);
    TEST_ASSERT_EQUAL_STRING("AUDUSD", ob->instrument);
}

void test_3(void) {
    BookList bl;
    OrderBook *ob, *ob2;
    
    TEST_ASSERT_EQUAL_PTR(&bl, BookList_init(&bl));

    ob = BookList_find(&bl, "AUDUSD");
    TEST_ASSERT_NOT_NULL(ob);
    TEST_ASSERT_EQUAL_INT(1, bl.size);
    TEST_ASSERT_EQUAL_STRING("AUDUSD", ob->instrument);
    ob2 = ob;

    ob = BookList_find(&bl, "GBPUSD");
    TEST_ASSERT_NOT_NULL(ob);
    TEST_ASSERT_EQUAL_INT(2, bl.size);
    TEST_ASSERT_EQUAL_STRING("GBPUSD", ob->instrument);

    /* Test the first ine is still there */
    ob = BookList_find(&bl, "AUDUSD");
    TEST_ASSERT_EQUAL_PTR(ob, ob2); /* assume no realloc for 2 members! */
    TEST_ASSERT_EQUAL_INT(2, bl.size);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_append);
    RUN_TEST(test_3);
    return UNITY_END();
}
