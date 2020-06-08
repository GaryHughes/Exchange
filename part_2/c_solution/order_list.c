/*
 * A list of orders, kept in a priority queue. 
 * Top of order book is orders[0]
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "order_list.h"

/* n-ary heap defines.  Default is binary */
#ifndef HEAP_FANOUT 
#define HEAP_FANOUT 2
#endif

OrderList *OrderList_init(OrderList *ol) {
    ol->capacity = 10;
    ol->size = 0;
    ol->orders = (Order *)malloc(ol->capacity * sizeof(Order));
    return ol->orders ? ol : NULL;
}

void OrderList_destroy(OrderList *ol) {
    free((void *)ol->orders);
}

static int generation = 0;

/* Comparison function - is a before b in the heap?
 * Looks at price then generation.
 * if ismin = false, price comparison is reversed but (for stability) generation is not
 * Inlined so hopefully constant folding will optimize this!
 */
static inline int is_before(const Order *a, const Order *b, int ismin) {
    double pdiff = a->price - b->price;
    if (ismin)
        return pdiff < 0 || (pdiff == 0.0 && a->generation < b->generation);
    else
        return pdiff > 0 || (pdiff == 0.0 && a->generation < b->generation); 
}

static inline int parent(int i) {
    return (i - 1) / HEAP_FANOUT;
}
static inline int first_child(int i) {
    return (i * HEAP_FANOUT) + 1;
}
static inline int last_child(int i) {
    return (i * HEAP_FANOUT) + HEAP_FANOUT;
}

/* Common to min and max, return NULL for malloc failure */
static inline Order *OrderList_push_impl(OrderList *ol, const char *id, int qty, double price, int ismin) {
    int pos, ppos;
    Order tmp;

    // assert(ol->size <= ol->capacity && ol->size >= 0);
    if (ol->size == ol->capacity) {
        /* increase by 1.5x */
        ol->capacity += ol->capacity >> 1;
        ol->orders = (Order *)realloc(ol->orders, ol->capacity * sizeof(Order));
        if (!ol->orders)
            return NULL;
    }
    strcpy(tmp.id, id);
    tmp.qty = qty;
    tmp.price = price;
    tmp.generation = ++generation;

    /* 
     * Restore the heap property.  Find the spot where tmp needs to be inserted,
     * starting at the new bottom of the heap.
     * This is usual heap "sift up" algorithm.
     */
    pos = ol->size++;
    while (pos > 0) {
        ppos = parent(pos);
        if (!is_before(&tmp, &ol->orders[ppos], ismin)) {
            ol->orders[pos] = tmp;
            return ol->orders + pos;
        }
        /* Move the parent down */
        ol->orders[pos] = ol->orders[ppos];
        pos = ppos;
    }
    /* Pushed the new parent, pos = 0 */
    ol->orders[0] =  tmp;
    return ol->orders;
}

Order *OrderList_push_min(OrderList *ol, const char *id, int qty, double price) {
    return OrderList_push_impl(ol, id, qty, price, 1);
}

Order *OrderList_push_max(OrderList *ol, const char *id, int qty, double price) {
    return OrderList_push_impl(ol, id, qty, price, 0);
}
    
static inline void OrderList_pop_impl(OrderList *ol, int ismin) {
    int pos = 0;
    Order tmp;

    if (!ol->size) return;
    /* Save the last element of heap, reduce size by 1 */
    tmp = ol->orders[--ol->size];
    /* 
     * Find where to insert this element, starting at the (nominally empty) root.
     * Re-establish the heap invariants. This is the usual heap "sift down"
     */
    while (1) {
        int smaller = pos;
        int fchild = first_child(pos);
        int lchild = last_child(pos);

        if (fchild >= ol->size) {
            /* Leaf node */
            ol->orders[pos] = tmp;
            return;
        }
        if (lchild >= ol->size)
            lchild = ol->size - 1;

        for (int i = fchild; i <= lchild; i++) {
            if (is_before(&ol->orders[i], smaller == pos ? &tmp : ol->orders + smaller, ismin))
                smaller = i;
        }
        if (smaller == pos) {
            ol->orders[pos] = tmp;
            return;
        }
        /* Move smallest child to parent */
        ol->orders[pos] = ol->orders[smaller];
        pos = smaller;
    }
}

void OrderList_pop_min(OrderList *ol) {
    OrderList_pop_impl(ol, 1);
}

void OrderList_pop_max(OrderList *ol) {
    OrderList_pop_impl(ol, 0);
}

/* Debugging / unit testing */
int OrderList_check_heap(const OrderList *ol, int ismin) {
    if (!ol->size) return 1;
    for (int pos = ol->size - 1; pos > 0; pos--) {
        int ppos = parent(pos); 
        if (is_before(ol->orders + pos, ol->orders + ppos, ismin)) {
            printf("check_heap failed, pos %d %g %d is before ppos %d %g %d\n",
                pos, ol->orders[pos].price, ol->orders[pos].generation,
                ppos, ol->orders[ppos].price, ol->orders[ppos].generation);
            return 0;
        }
    }
    return 1;
}
