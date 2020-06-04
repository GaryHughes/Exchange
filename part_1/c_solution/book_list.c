
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "book_list.h"

BookList *BookList_init(BookList *bl) {
    bl->capacity = 10;
    bl->size = 0;
    bl->books = (OrderBook *)malloc(bl->capacity * sizeof(OrderBook));
    return bl;
}

void BookList_destroy(BookList *bl) {
    for (int i = 0; i < bl->size; i++)
        OrderBook_destroy(bl->books + i);
    free(bl->books);
}

/* a compare function for bsearch() */
static int bl_find(const void *kp, const void *obp) {
    return strcmp((const char *)kp, ((const OrderBook *)obp)->instrument);
}

/* a compare function for qsort() */
static int bl_sort(const void *l, const void *r) {
    return strcmp(((const OrderBook *)l)->instrument, 
                    ((const OrderBook *)r)->instrument);
}

/* Find the OrderBook for the given instrument, creating it if needed */
OrderBook *BookList_find(BookList *bl, const char *instrument) {
    void *r = bsearch(instrument, bl->books, bl->size, sizeof(OrderBook), bl_find);
    if (r) return (OrderBook *)r;
    /* else need to insert it */
    if (bl->size == bl->capacity) {
        bl->capacity += bl->capacity >> 1;
        bl->books = (OrderBook *)realloc(bl->books, bl->capacity * sizeof(OrderBook));
        if (!bl->books) return NULL;
    }
    OrderBook_init(bl->books + bl->size, instrument);
    bl->size++;
    qsort(bl->books, bl->size, sizeof(OrderBook), bl_sort);
    r = bsearch(instrument, bl->books, bl->size, sizeof(OrderBook), bl_find);
    assert(r); /* Must be there, we just added it! */
    return (OrderBook *)r;
}
