/*
 * Implement the OrderBook.  Keep buys & sells for a single instrument.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "order_book.h"

static int next_generation() {
    static int gen = 0;
    return ++gen;
}

OrderBook *OrderBook_init(OrderBook *ob, const char *instrument) {
    if (OrderList_init(&ob->buys) && OrderList_init(&ob->sells)) {
        strlcpy(ob->instrument, instrument, INSTRUMENT_SIZE);
        ob->best_bid = 0;
        ob->best_offer = 1e99;
        return ob;
    }
    return NULL;
}

void OrderBook_destroy(OrderBook *ob) {
    OrderList_destroy(&ob->buys);
    OrderList_destroy(&ob->sells);
}

OrderBook *OrderBook_append(OrderBook *ob, const char *id, int qty, double price) {
    Order o;
    Order *ret;

    strcpy(o.id, id);
    o.price = price;
    o.generation = next_generation();

    if (qty > 0) {
        o.qty = qty;
        ret = OrderList_append(&ob->buys, &o);
        if (o.price > ob->best_bid)
            ob->best_bid = o.price;
    } else {
        o.qty = -qty;
        ret = OrderList_append(&ob->sells, &o);
        if (o.price < ob->best_offer)
            ob->best_offer = o.price;
    }
    return ret ? ob : NULL;
}

void OrderBook_sort(OrderBook *ob) {
    OrderList_sort_descending(&ob->buys);
    OrderList_sort_ascending(&ob->sells);
}

bool OrderBook_match(OrderBook *ob, Trade *t) {
    Order *bp, *sp;

    if (ob->best_bid < ob->best_offer)
        return false;

    if (!ob->buys.size || !ob->sells.size)
        return false;
    
    OrderBook_sort(ob);

    bp = ob->buys.orders;
    sp = ob->sells.orders;

    /* Have an actual trade */
    strcpy(t->buyer, bp->id);
    strcpy(t->seller, sp->id);
    t->price = bp->generation < sp->generation ? bp->price : sp->price;
    if (bp->qty > sp->qty) {
        t->qty = sp->qty;
        bp->qty -= sp->qty;
        OrderList_remove_first(&ob->sells);
        ob->best_offer = ob->sells.orders[0].price;
    } else if (sp->qty > bp->qty) {
        t->qty = bp->qty;
        sp->qty -= bp->qty;
        OrderList_remove_first(&ob->buys);
        ob->best_bid = ob->buys.orders[0].price;
    } else {
        t->qty = bp->qty;
        OrderList_remove_first(&ob->buys);
        OrderList_remove_first(&ob->sells);
        ob->best_offer = ob->sells.orders[0].price;
        ob->best_bid = ob->buys.orders[0].price;
    }
    return true;
}

/* Helper function for unit testing */
Order * Order_init(Order *o, const char *id, int qty, double price) {
    strcpy(o->id, id);
    o->qty = qty;
    o->price = price;
    o->generation = next_generation();
    return o;
}
