/* 
 * A single order book, buys & sells for a single instrument 
 */

#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <stdbool.h>

#include "exchange_defs.h"
#include "order_list.h"

typedef struct {
    OrderList buys, sells;
    char instrument[INSTRUMENT_SIZE];
} OrderBook;

OrderBook *OrderBook_init(OrderBook *ob, const char *instrument); /* NULL if an error from malloc() */
void OrderBook_destroy(OrderBook *ob);

/* Returns pointer to the original order o, NULL on error */
const Order *OrderBook_append(OrderBook *ob, const Order *o);

/* See if there is a trade to match.  If so, fill in t and return true */
bool OrderBook_match(OrderBook *, Trade *);
#endif /* ORDER_BOOK_H */