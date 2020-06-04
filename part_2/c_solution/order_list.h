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

/* 
 * Push a new entry on the orders heap.  Heap is on price+generation (for stability)
 * Return the address of the added order in the list, or NULL on error. 
 * Don't keep this pointer around, it will be invalidated by another append operation.
 * One version each for min heap (sells) and max heap (buys)
 */
Order *OrderList_push_max(OrderList *ol, const char *id, int qty, double price);
Order *OrderList_push_min(OrderList *ol, const char *id, int qty, double price);

/* 
 * heap pop() - Remove the Order at the top of the heap, reducing the size by 1.
 * One version for min heap (sells), one for max heap (buys)
 */
void OrderList_pop_min(OrderList *ol);
void OrderList_pop_max(OrderList *ol);

/* For unit testing */
/* Return 1 if it really is still a heap */
int OrderList_check_heap(const OrderList *ol, int ismin);
#endif /* ORDER_LIST_H */
