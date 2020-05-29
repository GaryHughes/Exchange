/*
 * Main routine
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "order_book.h"
#include "book_list.h"

int main()
{
    char line[LINE_SIZE];
    BookList bl;
    OrderBook *ob;
    Trade t;
    char instrument[INSTRUMENT_SIZE];
    char id[ID_SIZE];
    int qty;
    double price;

    /* dynamically create the order format so it knows ID_SIZE and INSTRUMENT_SIZE */
    char order_format[LINE_SIZE];
    snprintf(order_format, sizeof(order_format), 
                "%%%d[^:]:%%%d[^:]:%%d:%%lf", ID_SIZE - 1, INSTRUMENT_SIZE - 1);
    
    if (!BookList_init(&bl)) {
        fprintf(stderr, "Can't init BookList\n");
        exit(1);
    }

    while (fgets(line, sizeof(line), stdin)) {
        if (isspace(line[0]))
            /* last line in the file is empty */
            break;
 
        /* A:AUDUSD:­-100:1.47 */
        /* Char Sizes copied from exchange_defs.h....
        don't have ability to set this in function args as with printf()*/
        int ret = sscanf(line, order_format, id, instrument, &qty, &price);
        if (ret != 4) {
           fprintf(stderr, "Can't parse input line [%s]\n", line);
           exit(1);
        }
        id[ID_SIZE - 1] = 0;
        instrument[INSTRUMENT_SIZE - 1] = 0;

        ob = BookList_find(&bl, instrument);
        if (!ob){
            fprintf(stderr, "Can't add instument %s to BookList\n", instrument);
            exit(1);
        }
        OrderBook_append(ob, id, qty, price);
        while (OrderBook_match(ob, &t))
            printf("%s:%s:%s:%d:%g\n", t.buyer, t.seller, instrument, t.qty, t.price);
    }
    BookList_destroy(&bl);
    return 0;
}