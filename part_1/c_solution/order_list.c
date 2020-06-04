/*
 * Implement the Order type.  Not much here!
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "order_list.h"

OrderList *OrderList_init(OrderList *ol) {
    ol->capacity = 10;
    ol->size = 0;
    ol->orders = (Order *)malloc(ol->capacity * sizeof(Order));
    return ol->orders ? ol : NULL;
}

void OrderList_destroy(OrderList *ol) {
    free((void *)ol->orders);
}

Order *OrderList_append(OrderList *ol, const Order *o) {
    assert(ol->size <= ol->capacity && ol->size >= 0);
    if (ol->size == ol->capacity) {
        /* increase by 1.5x */
        ol->capacity += ol->capacity >> 1;
        ol->orders = (Order *)realloc(ol->orders, ol->capacity * sizeof(Order));
        if (!ol->orders)
            return NULL;
    }
    ol->orders[ol->size] = *o;
    return ol->orders + ol->size++;
}

static int compare_ascending(const void *l, const void *r) {
    const Order *lp = (const Order *)l;
    const Order *rp = (const Order *)r;

    if (lp->price < rp->price) return -1;
    if (lp->price > rp->price) return 1;
    return lp->generation - rp->generation;
}

static int compare_descending(const void *l, const void *r) {
    const Order *lp = (const Order *)l;
    const Order *rp = (const Order *)r;

    if (lp->price > rp->price) return -1;
    if (lp->price < rp->price) return 1;
    return lp->generation - rp->generation;
}

void OrderList_sort_ascending(OrderList *ol) {
    qsort(ol->orders, ol->size, sizeof(Order), compare_ascending);
}

void OrderList_sort_descending(OrderList *ol) {
    qsort(ol->orders, ol->size, sizeof(Order), compare_descending);
}

void OrderList_remove_first(OrderList *ol) {
    ol->size--;
    memmove(ol->orders, ol->orders + 1, ol->size * sizeof(Order));
}

