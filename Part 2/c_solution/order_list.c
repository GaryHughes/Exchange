/*
 * A list of orders, kept in sorted order. 
 * Toop of order book is orders[0]
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

Order *OrderList_append(OrderList *ol, const char *id, int qty, double price) {
    Order *op;

    // assert(ol->size <= ol->capacity && ol->size >= 0);
    if (ol->size == ol->capacity) {
        /* increase by 1.5x */
        ol->capacity += ol->capacity >> 1;
        ol->orders = (Order *)realloc(ol->orders, ol->capacity * sizeof(Order));
        if (!ol->orders)
            return NULL;
    }
    op = ol->orders + ol->size++;
    strcpy(op->id, id);
    op->qty = qty;
    op->price = price;
    return op;
}

int OrderList_remove_find_lowest(OrderList *ol, int idx) {
    double minp;
    int minidx;

    memmove(&ol->orders[idx], &ol->orders[idx+1], (ol->size - idx - 1) * sizeof(Order));
    if (!--ol->size)
        return -1;
    minp = ol->orders[0].price;
    minidx = 0;
    for (int i = 1; i < ol->size; i++) {
        if (ol->orders[i].price < minp) {
            minp = ol->orders[i].price;
            minidx = i;
        }
    }
    return minidx;
}

int OrderList_remove_find_highest(OrderList *ol, int idx) {
    double maxp;
    int maxidx;

    memmove(&ol->orders[idx], &ol->orders[idx+1], (ol->size - idx - 1) * sizeof(Order));
    if (!--ol->size)
        return -1;
    maxp = ol->orders[0].price;
    maxidx = 0;
    for (int i = 1; i < ol->size; i++) {
        if (ol->orders[i].price > maxp) {
            maxp = ol->orders[i].price;
            maxidx = i;
        }
    }
    return maxidx;
}
