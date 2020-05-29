/*
 * Types and constants for the Exchange simulator
 */

#ifndef EXCHANGE_DEFS
#define EXCHANGE_DEFS

#define LINE_SIZE 1000 /* Max size of input line */
#define ID_SIZE 4 /* Max size of buyer / seller ID */
#define INSTRUMENT_SIZE 8 /* Max size of instrument type */

/* Some utility types that dont need their own files */
typedef struct {
    char id[ID_SIZE]; /* buyer/seller ID */
    int qty;
    double price;
    int generation;
} Order;

typedef struct {
    char buyer[ID_SIZE];
    char seller[ID_SIZE];
    int qty;
    double price;
} Trade;

#endif /* EXCHANGE_DEFS */
