#! /usr/bin/env python3

# cython: language_level=3
# c_string_encoding=ascii

from cpython.mem cimport PyMem_Malloc, PyMem_Realloc, PyMem_Free

from libc.string cimport memmove, strncpy
from libc.stdlib cimport qsort

cdef class Trade:
    '''
    No logic here, just a container that can be string-d
    '''
    cdef readonly str buyer, seller, instrument
    cdef readonly int qty
    cdef readonly double price

    def __init__(self, str buyer, str seller, str instrument, 
                    int qty, double price):
        self.buyer = buyer
        self.seller = seller
        self.instrument = instrument
        self.qty = qty
        self.price = price

    def __str__(self):
        return ':'.join((self.buyer, self.seller, self.instrument, 
                str(self.qty), str(self.price)))

cdef int next_generation = 0;
cdef int generation():
    global next_generation
    next_generation += 1
    return next_generation

cdef struct Order:
    # Represent an order, no logic just a container
    char id[8]
    int qty
    double price
    int generation

cdef int compare_increasing(const void *lv, const void *rv) nogil:
    cdef const Order *l = <const Order *>lv
    cdef const Order *r = <const Order *>rv
    if l.price < r.price: return -1
    if l.price > r.price: return 1
    if l.generation < r.generation: return -1
    if l.generation > r.generation: return 1
    return 0

cdef int compare_decreasing(const void *lv, const void *rv) nogil:
    cdef const Order *l = <const Order *>lv
    cdef const Order *r = <const Order *>rv
    if l.price > r.price: return -1
    if l.price < r.price: return 1
    if l.generation < r.generation: return -1
    if l.generation > r.generation: return 1
    return 0

cdef struct OrderList:
    Order *orders 
    size_t size, capacity

cdef void cinit_OrderList(OrderList *self):
    self.capacity = 10
    self.size = 0
    self.orders = <Order *>PyMem_Malloc(self.capacity * sizeof(Order))
    if not self.orders:
        raise MemoryError()

cdef void dealloc_OrderList(OrderList *self):
    PyMem_Free(<void *>self.orders)

cdef void remove_first_OrderList(OrderList *self):
    memmove(self.orders, self.orders + 1, (self.size - 1) * sizeof(Order))
    self.size -= 1

cdef void append_OrderList(OrderList *self, str id, int qty, double price):
    if self.size == self.capacity:
        # extend by 1.5
        self.capacity += (self.capacity >> 1)
        self.orders = <Order *>PyMem_Realloc(<void *>self.orders, self.capacity * sizeof(Order))
        if not self.orders:
            raise MemoryError()

    cdef Order *p = &self.orders[self.size]
    strncpy(p.id, id.encode('utf8'), sizeof(p.id))
    p.id[sizeof(p.id)-1] = 0
    p.qty = qty
    p.price = price
    p.generation = generation()
    self.size += 1

cdef class OrderBook:
    cdef readonly str instrument
    cdef OrderList buys
    cdef OrderList sells
    cdef double best_bid
    cdef double best_offer
    cdef bint last_buy

    def __cinit__(self):
        cinit_OrderList(&self.buys)
        cinit_OrderList(&self.sells)

    def __deallocate__(self):
        dealloc_OrderList(&self.buys)
        dealloc_OrderList(&self.sells)

    def __init__(self, str instrument):
        self.instrument = instrument
        self.best_bid = 0.0
        self.best_offer = 1e10
    
    cpdef append(self, str id, int qty, double price):
        if qty >= 0:
            self.last_buy = True
            append_OrderList(&self.buys, id, qty, price)
            if price > self.best_bid:
                self.best_bid = price
        else:
            self.last_buy = False
            append_OrderList(&self.sells, id, -qty, price)
            if price < self.best_offer:
                self.best_offer = price

    cpdef list match(self):
        '''
        The guts of the exchange matching logic
        returns a list ot Trade objects
        '''
        cdef list ret
        cdef int tqty
        cdef double tprice
        cdef Order *buy
        cdef Order *sell
        cdef str buyid, sellid

        ret = []

        if self.best_bid < self.best_offer:
            return ret

        self.sort_books()
        #self.print_books("before")
        while self.buys.size and self.sells.size and self.best_bid >= self.best_offer:
            buy = self.buys.orders
            sell = self.sells.orders
            buyid = buy.id.decode('utf-8')
            sellid = sell.id.decode('utf-8')

            tprice = sell.price if self.last_buy else buy.price

            if buy.qty > sell.qty:
                tqty = sell.qty
                remove_first_OrderList(&self.sells)
                self.best_offer = self.sells.orders[0].price if self.sells.size else 1e10
                buy.qty -= tqty
            elif sell.qty > buy.qty:
                tqty = buy.qty
                remove_first_OrderList(&self.buys)
                self.best_bid = self.buys.orders[0].price if self.buys.size else 0.0
                sell.qty -= tqty
            else: # equal
                tqty = buy.qty
                remove_first_OrderList(&self.buys)
                remove_first_OrderList(&self.sells)
                self.best_bid = self.buys.orders[0].price if self.buys.size else 0.0
                self.best_offer = self.sells.orders[0].price if self.sells.size else 1e10
            ret.append(Trade(buyid, sellid, self.instrument, tqty, tprice))
            #self.print_books("after")
        return ret

    def sort_books(self):
        '''
        Extract this into a separate function so we can use it in the unit tests
        '''
        qsort(self.buys.orders, self.buys.size, sizeof(Order), &compare_decreasing)
        qsort(self.sells.orders, self.sells.size, sizeof(Order), &compare_increasing)

    def print_books(self, msg):
        if msg:
            print(msg)
        print(self.buys.size, "buys, best", self.best_bid)
        print(self.top_buys())
        print(self.sells.size, "sells, best", self.best_offer)
        print(self.top_sells())

    # These functions are for the unit tests
    def num_buys(self):
        return self.buys.size
    def num_sells(self):
        return self.sells.size
    def buy_qty(self, n=0):
        if self.buys.size > n:
            return self.buys.orders[n].qty
        else:
            return 0
    def sell_qty(self, n=0):
        if self.sells.size > n:
            return self.sells.orders[n].qty
        else:
            return 0
    def top_buys(self, n=5):
        ret = ""
        for i in range(n):
            if i >= self.buys.size:
                break
            ret += "%2d: %s %8d %9.6f %4d\n" % (i, self.buys.orders[i].id, 
                        self.buys.orders[i].qty, self.buys.orders[i].price, 
                        self.buys.orders[i].generation )
        return ret
    def top_sells(self, n=5):
        ret = ""
        for i in range(n):
            if i >= self.sells.size:
                break
            ret += "%2d: %s %8d %9.6f %4d\n" % (i, self.sells.orders[i].id,
                        self.sells.orders[i].qty, self.sells.orders[i].price,
                        self.sells.orders[i].generation )
        return ret

def main():
    import sys
    books = {}
    for l in sys.stdin:
        if len(l) < 2: break # empty last line
        id, instrument, qty, price = l.strip().split(":")
        try:
            b = books[instrument]
        except KeyError:
            b = books[instrument] = OrderBook(instrument)
        b.append(id, int(qty), float(price))
        for t in b.match():
            print(t)

if __name__ == "__main__":
    main()
