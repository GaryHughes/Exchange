/*
 * Basic order type
 */

#ifndef ORDER_H
#define ORDER_H

#include "exchange_defs.h"

typedef struct {
    char id[ID_SIZE]; /* buyer/seller ID */
    char instrument[INSTRUMENT_SIZE];
    char side; /* B or S */
    int qty;
    double price;
    int generation;
} Order;

/* Fill in an order from the input line */
/* Returns the first argument, or NULL on error */
extern Order *Order_create(Order *, const char *s); 

#endif /* ORDER_H */