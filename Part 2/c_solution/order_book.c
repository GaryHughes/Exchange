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
        strncpy(ob->instrument, instrument, INSTRUMENT_SIZE - 1);
        ob->instrument[INSTRUMENT_SIZE-1] = 0;
        return ob;
    }
    return NULL;
}

void OrderBook_destroy(OrderBook *ob) {
    OrderList_destroy(&ob->buys);
    OrderList_destroy(&ob->sells);
}

OrderBook *OrderBook_append(OrderBook *ob, const char *id, int qty, double price) {
    Order *ret;

    if (qty > 0) {
        ob->last_order_buy = true;
        ret = OrderList_push_max(&ob->buys, id, qty, price);
    } else {
        ob->last_order_buy = false;
        qty = -qty;
        ret = OrderList_push_min(&ob->sells, id, qty, price);
    }
    return ret ? ob : NULL;
}

bool OrderBook_match(OrderBook *ob, Trade *t) {
    
    if (!ob->buys.size || !ob->sells.size)
        return false;
    if (ob->buys.orders[0].price < ob->sells.orders[0].price)
        return false;
    
    /* Have an actual trade */
    strcpy(t->buyer, ob->buys.orders[0].id);
    strcpy(t->seller, ob->sells.orders[0].id);
    t->price = ob->last_order_buy ? ob->sells.orders[0].price : ob->buys.orders[0].price;
    if (ob->sells.orders[0].qty > ob->buys.orders[0].qty) {
        t->qty = ob->buys.orders[0].qty;
        ob->sells.orders[0].qty -= ob->buys.orders[0].qty;
        OrderList_pop_max(&ob->buys);
    } else if (ob->buys.orders[0].qty > ob->sells.orders[0].qty) {
        t->qty = ob->sells.orders[0].qty;
        ob->buys.orders[0].qty -= ob->sells.orders[0].qty;
        OrderList_pop_min(&ob->sells);
    } else {
        t->qty = ob->buys.orders[0].qty;
        OrderList_pop_max(&ob->buys);
        OrderList_pop_min(&ob->sells);
    }
    return true;
}
