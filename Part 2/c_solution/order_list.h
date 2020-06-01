/* 
 * A list of Orders 
 */

#ifndef ORDER_LIST_H
#define ORDER_LIST_H

#include "exchange_defs.h"

typedef struct {
    Order *orders;
    int size, capacity;
} OrderList;

/* An order with this price is not valid */
#define MIN_PRICE 0.0
#define MAX_PRICE 1e99

OrderList *OrderList_init(OrderList *ol); /* NULL if it fails due to malloc() */
void OrderList_destroy(OrderList *ol);

/* Return the address of the added order in the list, or NULL on error.
 don't keep this pointer around, it will be invalidated by another append operation */
Order *OrderList_append(OrderList *ol, const char *id, int qty, double price);

/* 
 * Remove the Order at poisition idx, reducing the size by 1.
 * Then return the index of the first order in the list with the price 
 * that is the lowest in the list, or -1 if the list is empty.
 * This is a linear seach through the whole unsorted list, and does a memmove()
 * to remove the item. But good news is no sorting required, and it is "stable" - the
 * orders for the current lowest price are returned FIFO.
 */
int OrderList_remove_find_lowest(OrderList *ol, int idx);
int OrderList_remove_find_highest(OrderList *ol, int idx);

#endif /* ORDER_LIST_H */
