/* 
 * A list of Orders 
 */

#ifndef ORDER_LIST_H
#define ORDER_LIST_H

#include "exchange_defs.h"

typedef struct {
    Order *orders;
    int size, capacity, last_sorted_size;
} OrderList;

OrderList *OrderList_init(OrderList *ol); /* NULL if it fails due to malloc() */
void OrderList_destroy(OrderList *ol);

/* Return the address of the added order in the list, or NULL on error.
 don't keep this pointer around, it will be invalidated by another append operation */
Order *OrderList_append(OrderList *ol, const Order *o);

/* Remove the top element of the OrderList */
void OrderList_remove_first(OrderList *ol);

/* Two sort functions. Key is price (ascending/decending) then generation ascending */
void OrderList_sort_ascending(OrderList *ol);
void OrderList_sort_descending(OrderList *ol);

#endif /* ORDER_LIST_H */