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
    ol->last_sorted_size = 0;
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

/*
 * Hand-crafted stable insertion sort.
 * Requirements:
 * Entries from 0 .. (last_sorted_size -1) are already sorted and do not need to be re-ordered
 * Entries from last_sorted_size .. (size - 1) need to be "bubbled down" into the correct position
 * Sort must be stable so bubble-down must insert *after* all entries that compare equal
 * We can binary-search the already-sorted entries to find the place to inseet.
 *   (and we can shortcut a few of the common cases to make this faster)
 */

void OrderList_sort_ascending(OrderList *ol) {
    Order otmp;
    double price;
    int ni;

    if (!ol->size || ol->size == ol->last_sorted_size) 
        return;
    if (!ol->last_sorted_size)
        ol->last_sorted_size = 1; /* List of 1 element is already sorted! */
    while (ol->last_sorted_size < ol->size) {
        /* 0 .. last_sorted_size-1 are in ascending order */
        price = ol->orders[ol->last_sorted_size].price;
        /* special-case some edge cases */
        if (price < ol->orders[0].price) {
            /* Insert at head, is smaller than all existing */
            /* move 0..last_sorted_size-1 to 1..last_sorted_size */
            /* need to move last_sorted_size elements, element at last_sorted_size is already in otmp */
            otmp = ol->orders[ol->last_sorted_size];
            memmove(ol->orders + 1, ol->orders, ol->last_sorted_size * sizeof(Order));
            ol->orders[0] = otmp;
            ol->last_sorted_size++;
            continue;
        }
        if (price >= ol->orders[ol->last_sorted_size -1].price) {
            /* already bigger/equal than all the previous, so already in the right place, no move / insert required */
            ol->last_sorted_size++;
            continue;
        }
        /* 
         * Need to find ni, the index 1..last_sorded_size-1 *before* which
         * the new element gets inserted.
         */
        for (ni = ol->last_sorted_size - 1; ni > 0; --ni)
            if (ol->orders[ni-1].price <= price)
                break;
        // printf("Sort %lx asc size %d last_sorted %d new %d newprice %g\n", 
        //     (unsigned long)ol, ol->size, ol->last_sorted_size, ni, otmp.price);
        // for (int i = 0; i <= ol->last_sorted_size; ++i)
        //     printf("%2d %g\n", i, ol->orders[i].price);
        /* ni=0 and ni=last_sorted_size are special-cased above */
        assert(ni > 0 && ni < ol->last_sorted_size);
        /* move ni .. last_sorted_size-1 up to ni+1 .. last_sorted_size */
        otmp = ol->orders[ol->last_sorted_size];
        memmove(ol->orders + ni + 1, ol->orders + ni, (ol->last_sorted_size - ni) * sizeof(Order));
        /* Insert new element at ni */
        ol->orders[ni] = otmp;
        ol->last_sorted_size++;
    }
}

void OrderList_sort_descending(OrderList *ol) {
    Order otmp;
    double price;
    int ni;

    if (!ol->size || ol->size == ol->last_sorted_size) 
        return;
    if (!ol->last_sorted_size)
        ol->last_sorted_size = 1; /* List of 1 element is already sorted! */
    while (ol->last_sorted_size < ol->size) {
        /* 0 .. last_sorted_size-1 are in descending order */
        price = ol->orders[ol->last_sorted_size].price;
        /* special-case some edge cases */
        if (price > ol->orders[0].price) {
            /* Insert at head, is larger than all existing */
            /* move 0..last_sorted_size-1 to 1..last_sorted_size */
            /* need to move last_sorted_size elements, element at last_sorted_size is already in otmp */
            otmp = ol->orders[ol->last_sorted_size];
            memmove(ol->orders + 1, ol->orders, ol->last_sorted_size * sizeof(Order));
            ol->orders[0] = otmp;
            ol->last_sorted_size++;
            continue;
        }
        if (price <= ol->orders[ol->last_sorted_size -1].price) {
            /* already smaller/equal than all the previous, so already in the right place, no move / insert required */
            ol->last_sorted_size++;
            continue;
        }
        /* 
         * Need to find ni, the index 1..last_sorded_size-1 *before* which
         * the new element gets inserted.
         */
        for (ni = ol->last_sorted_size - 1; ni > 0; --ni)
            if (ol->orders[ni-1].price >= price)
                break;
        // printf("Sort %lx desc size %d last_sorted %d new %d newprice %g\n", 
        //     (unsigned long)ol, ol->size, ol->last_sorted_size, ni, otmp.price);
        // for (int i = 0; i <= ol->last_sorted_size; ++i)
        //     printf("%2d %g\n", i, ol->orders[i].price);
        /* ni=0 and ni=last_sorted_size are special-cased above */
        assert(ni > 0 && ni < ol->last_sorted_size);
        /* move ni .. last_sorted_size-1 up to ni+1 .. last_sorted_size */
        otmp = ol->orders[ol->last_sorted_size];
        memmove(ol->orders + ni + 1, ol->orders + ni, (ol->last_sorted_size - ni) * sizeof(Order));
        /* Insert new element at ni */
        ol->orders[ni] = otmp;
        ol->last_sorted_size++;
    }
}

void OrderList_remove_first(OrderList *ol) {
    ol->size--;
    ol->last_sorted_size--;
    memmove(ol->orders, ol->orders + 1, ol->size * sizeof(Order));
}
