/*
 * Test program to show object sizes
 */

#include <stdio.h>

#include "order.h"
#include "order_list.h"
#include "order_book.h"
#include "book_list.h"

#define S(type) printf("Sizeof %-12s is %3lu\n", #type, sizeof(type))

int main() {
    S(Order);
    S(OrderList);
    S(OrderBook);
    S(BookList);
    return 0;
}