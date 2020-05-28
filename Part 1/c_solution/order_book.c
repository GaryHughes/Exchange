/*
 * Implement the OrderBook.  Keep buys & sells for a single instrument.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "order_book.h"

OrderBook *OrderBook_init(OrderBook *ob, const char *instrument) {
    if (OrderList_init(&ob->buys) && OrderList_init(&ob->sells)) {
        strlcpy(ob->instrument, instrument, INSTRUMENT_SIZE);
        return ob;
    }
    return NULL;
}

void OrderBook_destroy(OrderBook *ob) {
    OrderList_destroy(&ob->buys);
    OrderList_destroy(&ob->sells);
}

const Order *OrderBook_append(OrderBook *ob, const Order *o) {
    Order *ret;

    if (o->side == 'B') {
        ret = OrderList_append(&ob->buys, o);
        OrderList_sort_descending(&ob->buys);
    } else {
        ret = OrderList_append(&ob->sells, o);
        OrderList_sort_ascending(&ob->sells);
    }
    return ret ? o : NULL;
}

bool OrderBook_match(OrderBook *ob, Trade *t) {
    Order *bp, *sp;

    if (!ob->buys.size || !ob->sells.size)
        return false;
    bp = ob->buys.orders;
    sp = ob->sells.orders;
    if (bp->price < sp->price)
        return false;
    
    /* Have an actual trade */
    strcpy(t->buyer, bp->id);
    strcpy(t->seller, sp->id);
    t->price = bp->generation < sp->generation ? bp->price : sp->price;
    if (bp->qty > sp->qty) {
        t->qty = sp->qty;
        bp->qty -= sp->qty;
        OrderList_remove_first(&ob->sells);
    } else if (sp->qty > bp->qty) {
        t->qty = bp->qty;
        sp->qty -= bp->qty;
        OrderList_remove_first(&ob->buys);
    } else {
        t->qty = bp->qty;
        OrderList_remove_first(&ob->buys);
        OrderList_remove_first(&ob->sells);
    }
    return true;
}