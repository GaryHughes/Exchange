#! /usr/bin/env python3

# cython: language_level=3

# cdef char BUY = b'B'
# cdef char SELL = b'S'

cdef class Order:
    # Represent an order, no logic just a container
    cdef public int qty
    cdef readonly double price
    cdef readonly str id
    cdef readonly Py_UNICODE side

    def __init__(self, str id, int qty, double price):
        self.id = id
        self.qty = qty
        self.price = price
        if self.qty < 0:
            self.qty = -self.qty
            self.side = "S"
        else:
            self.side = "B"

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

cdef class OrderBook:
    cdef readonly str instrument
    cdef readonly list buys
    cdef readonly list sells
    cdef double best_bid
    cdef double best_offer
    cdef Py_UNICODE last_side

    def __init__(self, instrument):
        self.instrument = instrument
        # Top of the order book is First element
        self.buys = []
        self.sells = []
        self.best_bid = 0.0
        self.best_offer = 1e10
    
    def append(self, Order o):
        self.last_side = o.side
        if o.side == "B":
            self.buys.append(o)
            if o.price > self.best_bid:
                    self.best_bid = o.price
            # Higest price first, rest assumes stable sort()
            #self.buys.sort(reverse=True, key=lambda o: o.price)
        else:
            self.sells.append(o)
            if o.price < self.best_offer:
                self.best_offer = o.price
            # lowest price first, rest assumes stable sort()
            #self.sells.sort(key=lambda o: o.price)

    cpdef list match(self):
        '''
        The guts of the exchange matching logic
        returns a list ot Trade objects
        '''
        cdef list ret
        cdef int tqty
        cdef double tprice


        ret = []

        if self.best_bid < self.best_offer:
            return ret

        self.sort_books()
        while self.buys and self.sells and self.best_bid >= self.best_offer:
            buy = self.buys[0]
            sell = self.sells[0]
            # if buy.price < sell.price:
            #     break
            tprice = buy.price if self.last_side == "S" else sell.price
            if buy.qty > sell.qty:
                tqty = sell.qty
                self.sells.pop(0)
                self.best_offer = self.sells[0].price if self.sells else 1e10
                buy.qty -= tqty
            elif sell.qty > buy.qty:
                tqty = buy.qty
                self.buys.pop(0)
                self.best_bid = self.buys[0].price if self.buys else 0.0
                sell.qty -= tqty
            else: # equal
                tqty = buy.qty
                self.buys.pop(0)
                self.sells.pop(0)
                self.best_bid = self.buys[0].price if self.buys else 0.0
                self.best_offer = self.sells[0].price if self.sells else 1e10
            ret.append(Trade(buy.id, sell.id, self.instrument, tqty, tprice))
            
        return ret

    def sort_books(self):
        '''
        Extract this into a separate function so we can use it in the unit tests
        '''
        self.buys.sort(reverse=True, key=lambda o: o.price)
        self.sells.sort(key=lambda o: o.price)

def main():
    import sys
    books = {}
    for l in sys.stdin:
        if len(l) < 2: break # empty last line
        id, instrument, qty, price = l.strip().split(":")
        o = Order(id, int(qty), float(price))
        try:
            b = books[instrument]
        except KeyError:
            b = books[instrument] = OrderBook(instrument)
        b.append(o)
        for t in b.match():
            print(t)

if __name__ == "__main__":
    main()
