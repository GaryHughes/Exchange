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

cdef int before_min(const Order* l, const Order *r):
    cdef double cmp = l.price - r.price
    return cmp < 0 or (cmp == 0 and l.generation < r.generation)

cdef int before_max(const Order* l, const Order *r):
    cdef double cmp = l.price - r.price
    return cmp > 0 or (cmp == 0 and l.generation < r.generation)

# heap pop()
# orders[0] is empty, replace it with orders[size-1] and sift down
cdef void pop_OrderList_impl(OrderList *self, int (*before)(const Order *, const Order *)):
    cdef size_t pos = 0
    cdef size_t cpos
    cdef size_t smallest
    cdef Order tmp = self.orders[self.size - 1]
    self.size -= 1

    while True: 
        cpos = (pos << 1) + 1
        if cpos >= self.size:
            # leaf node
            break
        smallest = pos
        if before(&self.orders[cpos], &tmp):
            smallest = cpos
        cpos += 1
        if cpos < self.size and before(&self.orders[cpos], &tmp if smallest == pos else &self.orders[smallest]):
            smallest = cpos
        if smallest == pos:
            break
        self.orders[pos] = self.orders[smallest]
        pos = smallest
    self.orders[pos] = tmp

cdef void pop_OrderList_min(OrderList *self):
    pop_OrderList_impl(self, before_min)

cdef void pop_OrderList_max(OrderList *self):
    pop_OrderList_impl(self, before_max)

# heap push()
cdef void append_OrderList_impl(OrderList *self, str id, int qty, double price, int (*before)(const Order *, const Order *)):
    if self.size == self.capacity:
        # extend by 1.5
        self.capacity += (self.capacity >> 1)
        self.orders = <Order *>PyMem_Realloc(<void *>self.orders, self.capacity * sizeof(Order))
        if not self.orders:
            raise MemoryError()

    cdef Order tmp
    strncpy(tmp.id, id.encode('utf8'), sizeof(tmp.id))
    tmp.id[sizeof(tmp.id)-1] = 0
    tmp.qty = qty
    tmp.price = price
    tmp.generation = generation()
    self.size += 1

    # heap sift up
    cdef size_t pos = self.size - 1
    cdef size_t ppos
    while pos > 0:
        ppos = (pos - 1) >> 1
        if before(&self.orders[ppos], &tmp):
            break
        self.orders[pos] = self.orders[ppos]
        pos = ppos
    self.orders[pos] = tmp
    
cdef void append_OrderList_max(OrderList *self, str id, int qty, double price):
    append_OrderList_impl(self, id, qty, price, before_max)

cdef void append_OrderList_min(OrderList *self, str id, int qty, double price):
    append_OrderList_impl(self, id, qty, price, before_min)

cdef class OrderBook:
    cdef readonly str instrument
    cdef OrderList buys
    cdef OrderList sells
    cdef bint last_buy

    def __cinit__(self):
        cinit_OrderList(&self.buys)
        cinit_OrderList(&self.sells)

    def __deallocate__(self):
        dealloc_OrderList(&self.buys)
        dealloc_OrderList(&self.sells)

    def __init__(self, str instrument):
        self.instrument = instrument
    
    cpdef append(self, str id, int qty, double price):
        if qty >= 0:
            self.last_buy = True
            append_OrderList_max(&self.buys, id, qty, price)
        else:
            self.last_buy = False
            append_OrderList_min(&self.sells, id, -qty, price)
 
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

        while self.buys.size and self.sells.size:
            buy = self.buys.orders
            sell = self.sells.orders
            if buy.price < sell.price:
                break
            buyid = buy.id.decode('utf-8')
            sellid = sell.id.decode('utf-8')

            tprice = sell.price if self.last_buy else buy.price

            if buy.qty > sell.qty:
                tqty = sell.qty
                pop_OrderList_min(&self.sells)
                buy.qty -= tqty
            elif sell.qty > buy.qty:
                tqty = buy.qty
                pop_OrderList_max(&self.buys)
                sell.qty -= tqty
            else: # equal
                tqty = buy.qty
                pop_OrderList_max(&self.buys)
                pop_OrderList_min(&self.sells)
            ret.append(Trade(buyid, sellid, self.instrument, tqty, tprice))
        return ret

    # These functions are for the unit tests

    def print_books(self, msg):
        if msg:
            print(msg)
        print(self.buys.size, "buys, best", self.best_bid)
        print(self.top_buys())
        print(self.sells.size, "sells, best", self.best_offer)
        print(self.top_sells())

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

    def check_heap_buy(self, msg):
        cdef size_t pos
        cdef size_t ppos
        for pos in range(self.buys.size - 1, 0, -1):
            ppos = (pos - 1) >> 1
            if not before_max(&self.buys.orders[ppos], &self.buys.orders[pos]):
                print("%s buy heap failed size %d pos %d %g gen %d ppos %d %g gen %d" % (msg, self.buys.size, pos, self.buys.orders[pos].price, self.buys.orders[pos].generation,
                        ppos, self.buys.orders[ppos].price, self.buys.orders[ppos].generation))
                return False
        return True

    def check_heap_sell(self, msg):
        cdef size_t pos
        cdef size_t ppos
        for pos in range(self.sells.size - 1, 0, -1):
            ppos = (pos - 1) >> 1
            if not before_min(&self.sells.orders[ppos], &self.sells.orders[pos]):
                print("%s sell heap failed size %d pos %d %g gen %d ppos %d %g gen %d" % (msg, self.sells.size, pos, self.sells.orders[pos].price, self.sells.orders[pos].generation,
                        ppos, self.sells.orders[ppos].price, self.sells.orders[ppos].generation))
                return False
        return True

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
