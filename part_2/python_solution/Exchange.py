#! /usr/bin/env python3

import heapq

class SellOrder:
    '''
    Represent an order, no logic just a container
    '''
    gen = 0

    __slots__ = ['id', 'qty', 'price', 'generation']
    
    def __init__(self, id, qty, price):
        self.id = id
        self.qty = qty
        self.price = price
        SellOrder.gen += 1
        self.generation = SellOrder.gen

    def __lt__(self, r):
        return self.price < r.price \
                or (self.price == r.price and self.generation < r.generation)

class BuyOrder(SellOrder):
    # An order with the reversed price comparison
    def __lt__(self, r):
        return self.price > r.price \
                or (self.price == r.price and self.generation < r.generation)

class Trade:
    '''
    No logic here, just a container that can be string-d
    '''
    __slots__ = ['buyer', 'seller', 'instrument', 'qty', 'price']
    def __init__(self, buyer, seller, instrument, qty, price):
        self.buyer = buyer
        self.seller = seller
        self.instrument = instrument
        self.qty = qty
        self.price = price

    def __str__(self):
        return ':'.join((self.buyer, self.seller, self.instrument, 
                str(self.qty), str(self.price)))

class OrderBook:
    def __init__(self, instrument):
        self.instrument = instrument
        # Top of the order book is First element
        self.buys = []
        self.sells = []
        self.last_order = None
    
    def append(self, id, qty, price):
        qty = int(qty)
        price = float(price)
        if qty > 0:
            heapq.heappush(self.buys, BuyOrder(id, qty, price))
            self.last_order_buy = True
        else:
            heapq.heappush(self.sells, SellOrder(id, -qty, price))
            self.last_order_buy = False

    def match(self):
        '''
        The guts of the exchange matching logic
        returns a list ot Trade objects
        '''
        ret = []

        while self.buys and self.sells:
            buy = self.buys[0]
            sell = self.sells[0]
            if buy.price < sell.price:
                 break
            tprice = sell.price if self.last_order_buy else buy.price
            if buy.qty > sell.qty:
                tqty = sell.qty
                heapq.heappop(self.sells)
                buy.qty -= tqty
            elif sell.qty > buy.qty:
                tqty = buy.qty
                heapq.heappop(self.buys)
                sell.qty -= tqty
            else: # equal
                tqty = buy.qty
                heapq.heappop(self.buys)
                heapq.heappop(self.sells)
            ret.append(Trade(buy.id, sell.id, self.instrument, tqty, tprice))
            
        return ret

if __name__ == "__main__":
    import sys
    books = {}
    for l in sys.stdin:
        if len(l) < 2: break # empty last line
        id, instrument, qty, price = l.strip().split(":")
        try:
            b = books[instrument]
        except KeyError:
            b = books[instrument] = OrderBook(instrument)
        b.append(id, qty, price)
        for t in b.match():
            print(t)
