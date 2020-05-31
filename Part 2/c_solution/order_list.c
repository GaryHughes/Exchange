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
    ol->sorted_size = 0;
    ol->orders = (Order *)malloc(ol->capacity * sizeof(Order));
    return ol->orders ? ol : NULL;
}

void OrderList_destroy(OrderList *ol) {
    free((void *)ol->orders);
}

Order *OrderList_append(OrderList *ol, const Order *o) {
    // assert(ol->size <= ol->capacity && ol->size >= 0);
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

/*
 * Hand-crafted stable insertion sort.
 * Requirements:
 * Entries from 0 .. (sorted_size -1) are already sorted and do not need to be re-ordered
 * Entries from sorted_size .. (size - 1) need to be "bubbled down" into the correct position
 * Sort must be stable so bubble-down must insert *after* all entries that compare equal
 * We can binary-search the already-sorted entries to find the place to insert.
 *   (and we can shortcut a few of the common cases to make this faster)
 */

void OrderList_sort_ascending(OrderList *ol) {
    Order otmp;
    double price;
    int ni, lo, hi;

    if (!ol->size || ol->size == ol->sorted_size) 
        return;
    if (!ol->sorted_size)
        ol->sorted_size = 1; /* List of 1 element is already sorted! */
    while (ol->sorted_size < ol->size) {
        /* 0 .. sorted_size-1 are in ascending order */
        price = ol->orders[ol->sorted_size].price;
        /* special-case some edge cases */
        if (price < ol->orders[0].price) {
            /* Insert at head, is smaller than all existing */
            /* move 0..sorted_size-1 to 1..sorted_size */
            /* need to move sorted_size elements, & save element at sorted_size */
            otmp = ol->orders[ol->sorted_size];
            memmove(ol->orders + 1, ol->orders, ol->sorted_size * sizeof(Order));
            ol->orders[0] = otmp;
            ol->sorted_size++;
            continue;
        }
        if (price >= ol->orders[ol->sorted_size - 1].price) {
            /* already bigger/equal than all the previous, so already in the right place, no move / insert required */
            ol->sorted_size++;
            continue;
        }
        /* 
         * Need to find ni, the index 1..last_sorded_size-1 *before* which
         * the new element gets inserted.  This is the lowest index ni such that 
         *      orders[ni].price > price
         * We use a binary search for that to get O(n log n)
         */
        for (lo = 1, hi = ol->sorted_size - 1, ni = (lo + hi) >> 1;
            hi > lo;
            ni = (lo + hi) >> 1) {
                if (ol->orders[ni].price <= price)
                    lo = ni + 1;
                else
                    /* cant use -1 here as might need to insert just before ni */
                    hi = ni;
        }
        // assert(price < ol->orders[ni].price && price >= ol->orders[ni - 1].price);

        /* ni=0 and ni=sorted_size are special-cased above */
        // assert(ni > 0 && ni < ol->sorted_size);
        /* move ni .. sorted_size-1 up to ni+1 .. sorted_size */
        otmp = ol->orders[ol->sorted_size];
        memmove(ol->orders + ni + 1, ol->orders + ni, (ol->sorted_size - ni) * sizeof(Order));
        /* Insert new element at ni */
        ol->orders[ni] = otmp;
        ol->sorted_size++;
    }
}


void OrderList_sort_descending(OrderList *ol) {
    Order otmp;
    double price;
    int ni, lo, hi;

    if (!ol->size || ol->size == ol->sorted_size) 
        return;
    if (!ol->sorted_size)
        ol->sorted_size = 1; /* List of 1 element is already sorted! */
    while (ol->sorted_size < ol->size) {
        /* 0 .. sorted_size-1 are in ascending order */
        price = ol->orders[ol->sorted_size].price;
        /* special-case some edge cases */
        if (price > ol->orders[0].price) {
            /* Insert at head, is larger than all existing */
            /* move 0..sorted_size-1 to 1..sorted_size */
            /* need to move sorted_size elements, & save element at sorted_size */
            otmp = ol->orders[ol->sorted_size];
            memmove(ol->orders + 1, ol->orders, ol->sorted_size * sizeof(Order));
            ol->orders[0] = otmp;
            ol->sorted_size++;
            continue;
        }
        if (price <= ol->orders[ol->sorted_size - 1].price) {
            /* already bigger/equal than all the previous, so already in the right place, no move / insert required */
            ol->sorted_size++;
            continue;
        }
        /* 
         * Need to find ni, the index 1..last_sorded_size-1 *before* which
         * the new element gets inserted.  This is the lowest index ni such that 
         *      orders[ni].price < price
         */
        for (lo = 1, hi = ol->sorted_size - 1, ni = (lo + hi) >> 1;
            hi > lo;
            ni = (lo + hi) >> 1) {
                if (ol->orders[ni].price >= price)
                    lo = ni + 1;
                else
                    /* cant use -1 here as might need to insert just before ni */
                    hi = ni;
        }
        // assert(price > ol->orders[ni].price && price <= ol->orders[ni - 1].price);
        /* ni=0 and ni=sorted_size are special-cased above */
        // assert(ni > 0 && ni < ol->sorted_size);
        /* move ni .. sorted_size-1 up to ni+1 .. sorted_size */
        otmp = ol->orders[ol->sorted_size];
        memmove(ol->orders + ni + 1, ol->orders + ni, (ol->sorted_size - ni) * sizeof(Order));
        /* Insert new element at ni */
        ol->orders[ni] = otmp;
        ol->sorted_size++;
    }
}

void OrderList_remove_first(OrderList *ol) {
    ol->size--;
    ol->sorted_size--;
    memmove(ol->orders, ol->orders + 1, ol->size * sizeof(Order));
}
