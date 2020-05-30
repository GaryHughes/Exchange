/*
 * Implement the Order type.  Not much here!
 */

#include <stdio.h>

#include "order.h"

static int next_generation() {
    static int gen = 0;
    return ++gen;
}

Order *Order_create(Order *o, const char *s) {
    /* A:AUDUSD:­-100:1.47 */
    /* Char Sizes copied from exchange_defs.h....
     don't have ability to set this in function args as with printf()*/
    int ret = sscanf(s, "%7[^:]:%15[^:]:%d:%lf", o->id, o->instrument, &o->qty, &o->price);
    if (ret != 4)
        return NULL;
    o->id[ID_SIZE - 1] = 0;
    o->instrument[INSTRUMENT_SIZE - 1] = 0;
    if (o->qty > 0) {
        o->side = 'B';
    } else {
        o->side = 'S';
        o->qty = -o->qty;
    }
    o->generation = next_generation();
    return o;
}