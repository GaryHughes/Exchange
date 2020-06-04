/* 
 * A list of BookBooks, keyed by instrument.
 * 
 */

#ifndef BOOK_LIST_H
#define BOOK_LIST_H

#include "exchange_defs.h"
#include "order_book.h"


typedef struct {
    OrderBook *books;
    int size, capacity;
    /* Cache the result of the last lookup, as orders tend to be clustered */
    int previous;

} BookList;

BookList *BookList_init(BookList *bl); /* NULL if it fails due to malloc() */
void BookList_destroy(BookList *bl);

/* Find the OrderBook for the given instrument, creating it if needed */
OrderBook *BookList_find(BookList *bl, const char *instrument);

#endif /* BOOK_LIST_H */
