#! /usr/bin/env python3

import unittest
import Exchange

class TestOrderBook(unittest.TestCase):
    def test_create(self):
        o = Exchange.OrderBook("AUDUSD")
        self.assertEqual(o.instrument, "AUDUSD")
        self.assertEqual(o.num_buys(), 0)
        self.assertEqual(o.num_sells(), 0)
    
    def test_append(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append("A", 100, 1.47)
        self.assertEqual(b.num_buys(), 1)
        self.assertEqual(b.num_sells(), 0)
        self.assertEqual(b.buy_qty(), 100)

    def test_append_sell(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append("A", -100, 1.47)
        self.assertEqual(b.num_buys(), 0)
        self.assertEqual(b.num_sells(), 1)
        self.assertEqual(b.sell_qty(), 100)

class TestSorting(unittest.TestCase):
    def test_append_sort_buy_inorder(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append("A", 100, 1.47)
        b.append("A", 200, 1.47)
        self.assertEqual(b.num_buys(), 2)
        self.assertEqual(b.num_sells(), 0)
        # remember, first element is top of book
        self.assertEqual(b.buy_qty(), 100)
        self.assertEqual(b.buy_qty(1), 200)
       
    def test_append_sort_sell_inorder(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append("A", -100, 1.47)
        b.append("A", -200, 1.47)
        self.assertEqual(b.num_sells(), 2)
        self.assertEqual(b.num_buys(), 0)
        # remember, first element is top of book
        self.assertEqual(b.sell_qty(), 100)
        self.assertEqual(b.sell_qty(1), 200)
       
    def test_append_sort_buy_rev(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append("A", 200, 1.48)
        b.append("A", 100, 1.47)
        self.assertEqual(b.num_buys(), 2)
        self.assertEqual(b.num_sells(), 0)
        # for buys, first element has highest price
        self.assertEqual(b.buy_qty(), 200)
        self.assertEqual(b.buy_qty(1), 100)
       
    def test_append_sort_sell_rev(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append("A", -200, 1.48)
        b.append("A", -100, 1.47)
        self.assertEqual(b.num_sells(), 2)
        self.assertEqual(b.num_buys(), 0)
        # for sells, first element has lowest price
        self.assertEqual(b.sell_qty(), 100)
        self.assertEqual(b.sell_qty(1), 200)
       
class TestMatch(unittest.TestCase):
    def test_not_overlapped(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append("A", 200, 1.46)
        b.append("A", -100, 1.47)
        self.assertEqual(b.match(), [])


    def test_same_size(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append("CC", 100, 1.48)
        b.append("A", -100, 1.47)
        # Overlap, price is the first entry in the book
        tlist = b.match()
        self.assertEqual(len(tlist), 1)
        self.assertEqual(str(tlist[0]), "CC:A:AUDUSD:100:1.48")
        # check that the books have been updated
        self.assertEqual(b.num_sells(), 0)
        self.assertEqual(b.num_buys(), 0)
     
    def test_buy_bigger(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append("A", 200, 1.48)
        b.append("B", -100, 1.47)
        # Overlap, price is the first entry in the book
        tlist = b.match()
        self.assertEqual(len(tlist), 1)
        self.assertEqual(str(tlist[0]), "A:B:AUDUSD:100:1.48")
        # check that the books have been updated
        self.assertEqual(b.num_sells(), 0)
        self.assertEqual(b.num_buys(), 1)
        # buy should have 100 shs left
        self.assertEqual(b.buy_qty(), 100)

    def test_sell_bigger(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append("A", 100, 1.48)
        b.append("D", -200, 1.47)
        # Overlap, price is the first entry in the book
        tlist = b.match()
        self.assertEqual(len(tlist), 1)
        self.assertEqual(str(tlist[0]), "A:D:AUDUSD:100:1.48")
        # check that the books have been updated
        self.assertEqual(b.num_sells(), 1)
        self.assertEqual(b.num_buys(), 0)
        # sell should have 100 shs left
        self.assertEqual(b.sell_qty(), 100)

if __name__ == "__main__":
    unittest.main()
