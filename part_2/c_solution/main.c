/*
 * Main routine
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "order_book.h"
#include "book_list.h"

int main()
{
    char line[LINE_SIZE];
    BookList bl;
    OrderBook *ob;
    Trade t;
    char *instrument;
    char *id;
    char *p, *p2;
    int qty;
    double price;

    if (!BookList_init(&bl)) {
        fprintf(stderr, "Can't init BookList\n");
        exit(1);
    }

    while (fgets(line, sizeof(line), stdin)) {
        if (isspace(line[0]))
            /* last line in the file is empty */
            break;
 
        /* A:AUDUSD:­-100:1.47 */
        p = strchr(line, ':');
        if (!p) {
            fprintf(stderr, "Can't parse ID from input line %s", line);
            exit(1);
        }
        if (p - line >= ID_SIZE) {
            fprintf(stderr, "ID too long (max %d) from input line %s", ID_SIZE-1, line);
            exit(1);
        }
        id = line;
        *p++ = 0;
        /* instrument */
        instrument = p;
        p = strchr(p, ':');
         if (!p) {
            fprintf(stderr, "Can't parse instrument from input line %s", instrument);
            exit(1);
        }
        if (p - instrument >= INSTRUMENT_SIZE) {
            fprintf(stderr, "Instrument too long (max %d) from input line %s", INSTRUMENT_SIZE-1, instrument);
            exit(1);
        }
        *p++ = 0;
        /* Qty */
        qty = strtol(p, &p2, 10);
        if (*p2 != ':' || p2 == p) {
            fprintf(stderr, "Can't parse qty from input line %s", p);
            exit(1);
        }
        /* price */
        p2++;
        price = strtod(p2, &p);
        if (*p && !isspace(*p)) {
            fprintf(stderr, "Can't parse price from input line %s", p2);
            exit(1);

        }
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
