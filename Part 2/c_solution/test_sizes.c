/*
 * Test program to show object sizes
 */

#include <stdio.h>

#include "order_list.h"
#include "order_book.h"
#include "book_list.h"

#define S(type) printf("Sizeof %-12s is %3lu\n", #type, (unsigned long)sizeof(type))

int main() {
    S(OrderList);
    S(OrderBook);
    S(BookList);
    return 0;
}