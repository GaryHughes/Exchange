/*
 * Main routine
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "order.h"
#include "order_book.h"
#include "book_list.h"

int main()
{
    char line[LINE_SIZE];
    Order o;
    BookList bl;
    OrderBook *ob;
    Trade t;

    if (!BookList_init(&bl)) {
        fprintf(stderr, "Can't init BookList\n");
        exit(1);
    }

    while (fgets(line, sizeof(line), stdin)) {
        if (isspace(line[0]))
            /* last line in the file is empty */
            break;
        if (!Order_create(&o, line)) {
            fprintf(stderr, "Can't parse input line [%s]\n", line);
            exit(1);
        }
        ob = BookList_find(&bl, o.instrument);
        if (!ob){
            fprintf(stderr, "Can't add instument %s to BookList\n", o.instrument);
            exit(1);
        }
        OrderBook_append(ob, &o);
        while (OrderBook_match(ob, &t))
            printf("%s:%s:%s:%d:%g\n", t.buyer, t.seller, ob->instrument, t.qty, t.price);
    }
    BookList_destroy(&bl);
    return 0;
}