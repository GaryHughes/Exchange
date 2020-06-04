
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "book_list.h"

BookList *BookList_init(BookList *bl) {
    bl->capacity = 10;
    bl->size = 0;
    bl->books = (OrderBook *)malloc(bl->capacity * sizeof(OrderBook));
    bl->previous = 0;
    return bl;
}

void BookList_destroy(BookList *bl) {
    for (int i = 0; i < bl->size; i++)
        OrderBook_destroy(bl->books + i);
    free(bl->books);
}

/* Find the OrderBook for the given instrument, creating it if needed */
OrderBook *BookList_find(BookList *bl, const char *instrument) {
    int hi, lo, mid, cmp;

    /* First, check for empty */
    if (!bl->size) {
        bl->size = 1;
        bl->previous = 0;
        return OrderBook_init(bl->books, instrument);
    }

    /* Then, try the same as last lookup */
    cmp = strcmp(instrument, bl->books[bl->previous].instrument);
    if (!cmp)
        return bl->books + bl->previous;
    
    /* Use the compare vs the cached entry to seed bsearch */
    if (cmp > 0) {
        hi = bl->size; /* We may need to add at the end */
        lo = bl->previous + 1;
    } else {
        hi = bl->previous;
        lo = 0;
    }

    /* Using binary search, find the index of the first entry larger than instrument  */
    /* (assuming we don't find an exact!) */
    /* this may be hi which is off the end of the array, do don't ever dereference hi */
    while (hi > lo) {
        mid = (hi + lo) >> 1; /* always < hi */
        cmp = strcmp(instrument, bl->books[mid].instrument);
        if (!cmp) {
            /* Found id */
            bl->previous = mid;
            return bl->books + mid;
        }
        if (cmp < 0) {
            /* Can't use -1 here as we may need to insert exactly before mid */
            hi = mid;
        } else {
            /* Can use +1 here, as have to insert after mid */
            lo = mid + 1;
        }
    }

    /* 3 options here:
     - lo == size, so append at end (should never be > size.....)
     - lo < size && book[lo].instrument matches, return it
     - else must insert before book[lo].
     */
    bl->previous = lo;
    if (lo < bl->size && !strcmp(instrument, bl->books[lo].instrument))
        return bl->books + lo;

    /* Need to add, so expand if needed */
    if (bl->size == bl->capacity) {
        bl->capacity += bl->capacity >> 1;
        bl->books = (OrderBook *)realloc(bl->books, bl->capacity * sizeof(OrderBook));
        if (!bl->books) return NULL;
    }
    if (lo < bl->size) {
        /* need to insert it prior to lo, so move out of the way */
        memmove(bl->books + lo + 1, bl->books + lo, (bl->size - lo) * sizeof(OrderBook));
    } else {
        /* Is bigger than all other entries -> add at end, no need to move */
        assert(lo == bl->size);
    }
    bl->size++;
    return OrderBook_init(bl->books + lo, instrument);
}
