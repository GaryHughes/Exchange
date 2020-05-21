#! /usr/bin/env python3

class Order:
    '''
    Represent and order by splitting the input file appropriately
    generation is maintained as a class static
    '''
    def __init__(self, str):
        self.id, self.instrument, self.qty, self.price = str.strip().split(":")
        self.qty = int(self.qty)
        self.price = float(self.price)
        self.generation = Order.next_generation()
        if self.qty < 0:
            self.qty = -self.qty
            self.side = 'S'
        else:
            self.side = 'B'

    generation = 0
    @classmethod
    def next_generation(cls):
        cls.generation += 1
        return cls.generation

class Trade:
    '''
    No logic here, just a container that can be compared for equal
    '''
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
        # Top of the order book is LAST element
        self.buys = []
        self.sells = []
    
    def append(self, o):
        if o.side == "B":
            self.buys.append(o)
            # highet price then lowest generation last 
            self.buys.sort(key=lambda o: (o.price, -o.generation))
        else:
            self.sells.append(o)
            # lowest price then lowest generation last
            self.sells.sort(key=lambda o: (-o.price , -o.generation))

    def match(self):
        '''
        The guts of the exchange matching logic
        returns a list ot Trade objects
        '''
        ret = []

        while self.buys and self.sells:
            buy = self.buys[-1]
            sell = self.sells[-1]
            if buy.price < sell.price:
                break
            tprice = buy.price if buy.generation < sell.generation else sell.price
            if buy.qty > sell.qty:
                tqty = sell.qty
                self.sells.pop()
                buy.qty -= tqty
            elif sell.qty > buy.qty:
                tqty = buy.qty
                self.buys.pop()
                sell.qty -= tqty
            else: # equal
                tqty = buy.qty
                self.buys.pop()
                self.sells.pop()
            ret.append(Trade(buy.id, sell.id, buy.instrument, tqty, tprice))
            
        return ret

if __name__ == "__main__":
    import sys
    books = {}
    for l in sys.stdin:
        if len(l) < 2: break # empty last line due to \r\n
        o = Order(l)
        try:
            b = books[o.instrument]
        except KeyError:
            b = books[o.instrument] = OrderBook(o.instrument)
        b.append(o)
        for t in b.match():
            print(t)
