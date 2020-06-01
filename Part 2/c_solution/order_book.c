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
        ob->buy_price = MIN_PRICE;
        ob->sell_price = MAX_PRICE;
        ob->best_bid = ob->best_offer = -1;
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
        if (price > ob->buy_price) {
            /* Have new best bid */
            ob->buy_price = price;
            ob->best_bid =  ob->buys.size;
        }
        ret = OrderList_append(&ob->buys, id, qty, price);
    } else {
        ob->last_order_buy = false;
        qty = -qty;
        if (price < ob->sell_price) {
            /* Have new best offer */
            ob->sell_price = price;
            ob->best_offer = ob->sells.size;
        } 
        ret = OrderList_append(&ob->sells, id, qty, price);
    }
    return ret ? ob : NULL;
}

bool OrderBook_match(OrderBook *ob, Trade *t) {
    Order *bp, *sp;
    if (ob->buy_price < ob->sell_price)
        return false;

    bp = ob->buys.orders + ob->best_bid;
    sp = ob->sells.orders + ob->best_offer;
    /* Have an actual trade */
    strcpy(t->buyer, bp->id);
    strcpy(t->seller, sp->id);
    t->price = ob->last_order_buy ? ob->sell_price : ob->buy_price;
    if (sp->qty > bp->qty) {
        t->qty = bp->qty;
        sp->qty -= bp->qty;
        ob->best_bid = OrderList_remove_find_highest(&ob->buys, ob->best_bid);
        ob->buy_price = ob->best_bid >= 0 ? ob->buys.orders[ob->best_bid].price : MIN_PRICE;
    } else if (bp->qty > sp->qty) {
        t->qty = sp->qty;
        bp->qty -= sp->qty;
        ob->best_offer = OrderList_remove_find_lowest(&ob->sells, ob->best_offer);
        ob->sell_price = ob->best_offer >= 0 ? ob->sells.orders[ob->best_offer].price : MAX_PRICE;
    } else {
        t->qty = bp->qty;
        ob->best_bid = OrderList_remove_find_highest(&ob->buys, ob->best_bid);
        ob->buy_price = ob->best_bid >= 0 ? ob->buys.orders[ob->best_bid].price : MIN_PRICE;
        ob->best_offer = OrderList_remove_find_lowest(&ob->sells, ob->best_offer);
        ob->sell_price = ob->best_offer >= 0 ? ob->sells.orders[ob->best_offer].price : MAX_PRICE;
        }
    return true;
}
