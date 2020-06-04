#! /usr/bin/env python

import unittest
import Exchange

class TestOrderBook(unittest.TestCase):
    def test_create(self):
        o = Exchange.OrderBook("AUDUSD")
        self.assertEqual(o.instrument, "AUDUSD")
        self.assertEqual(o.buys, [])
        self.assertEqual(o.sells, [])
    
    def test_append(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append(Exchange.Order("A", "100", "1.47"))
        self.assertEqual(len(b.buys), 1)
        self.assertEqual(len(b.sells), 0)
        self.assertEqual(b.buys[0].qty, 100)

    def test_append_sell(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append(Exchange.Order("A", "-100", "1.47"))
        self.assertEqual(len(b.buys), 0)
        self.assertEqual(len(b.sells), 1)
        self.assertEqual(b.sells[0].qty, 100)

class TestSorting(unittest.TestCase):
    def test_append_sort_buy_inorder(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append(Exchange.Order("A", "100", "1.47"))
        b.append(Exchange.Order("A", "200", "1.47"))
        self.assertEqual(len(b.buys), 2)
        self.assertEqual(len(b.sells), 0)
        # remember, first element is top of book
        b.sort_books()
        self.assertEqual(b.buys[0].qty, 100)
        self.assertEqual(b.buys[1].qty, 200)
       
    def test_append_sort_sell_inorder(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append(Exchange.Order("A", "-100", "1.47"))
        b.append(Exchange.Order("A", "-200", "1.47"))
        self.assertEqual(len(b.sells), 2)
        self.assertEqual(len(b.buys), 0)
        # remember, first element is top of book
        b.sort_books()
        self.assertEqual(b.sells[0].qty, 100)
        self.assertEqual(b.sells[1].qty, 200)
       
    def test_append_sort_buy_rev(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append(Exchange.Order("A", "200", "1.48"))
        b.append(Exchange.Order("A", "100", "1.47"))
        self.assertEqual(len(b.buys), 2)
        self.assertEqual(len(b.sells), 0)
        # for buys, first element has highest price
        b.sort_books()
        self.assertEqual(b.buys[0].qty, 200)
        self.assertEqual(b.buys[1].qty, 100)
       
    def test_append_sort_sell_rev(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append(Exchange.Order("A", "-200", "1.48"))
        b.append(Exchange.Order("A", "-100", "1.47"))
        self.assertEqual(len(b.sells), 2)
        self.assertEqual(len(b.buys), 0)
        # for sells, first element has lowest price
        b.sort_books()
        self.assertEqual(b.sells[0].qty, 100)
        self.assertEqual(b.sells[1].qty, 200)
       
class TestMatch(unittest.TestCase):
    def test_not_overlapped(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append(Exchange.Order("A", "200", "1.46"))
        b.append(Exchange.Order("A", "-100", "1.47"))
        self.assertEqual(b.match(), [])


    def test_same_size(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append(Exchange.Order("A", "100", "1.48"))
        b.append(Exchange.Order("A", "-100", "1.47"))
        # Overlap, price is the first entry in the book
        tlist = b.match()
        self.assertEqual(len(tlist), 1)
        self.assertEqual(str(tlist[0]), "A:A:AUDUSD:100:1.48")
        # check that the books have been updated
        self.assertEqual(len(b.sells), 0)
        self.assertEqual(len(b.buys), 0)
     
    def test_buy_bigger(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append(Exchange.Order("A", "200", "1.48"))
        b.append(Exchange.Order("A", "-100", "1.47"))
        # Overlap, price is the first entry in the book
        tlist = b.match()
        self.assertEqual(len(tlist), 1)
        self.assertEqual(str(tlist[0]), "A:A:AUDUSD:100:1.48")
        # check that the books have been updated
        self.assertEqual(len(b.sells), 0)
        self.assertEqual(len(b.buys), 1)
        # buy should have 100 shs left
        self.assertEqual(b.buys[0].qty, 100)

    def test_sell_bigger(self):
        b = Exchange.OrderBook("AUDUSD")
        b.append(Exchange.Order("A", "100", "1.48"))
        b.append(Exchange.Order("A", "-200", "1.47"))
        # Overlap, price is the first entry in the book
        tlist = b.match()
        self.assertEqual(len(tlist), 1)
        self.assertEqual(str(tlist[0]), "A:A:AUDUSD:100:1.48")
        # check that the books have been updated
        self.assertEqual(len(b.sells), 1)
        self.assertEqual(len(b.buys), 0)
        # sell should have 100 shs left
        self.assertEqual(b.sells[0].qty, 100)

if __name__ == "__main__":
    unittest.main()