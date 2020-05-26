#! /usr/bin/env python3

import unittest
import Exchange

class TestTrade(unittest.TestCase):
    def test_create(self):
        t = Exchange.Trade('Buyer', 'Seller', 'BHP', 100, 2.34)
        self.assertEqual(t.buyer, 'Buyer')
        self.assertEqual(t.seller, 'Seller')
        self.assertEqual(t.instrument, 'BHP')
        self.assertEqual(t.qty,100)
        self.assertEqual(t.price, 2.34)

    def test_str(self):
        t = Exchange.Trade('Buyer', 'Seller', 'BHP', 100, 2.34)
        self.assertEqual(str(t), "Buyer:Seller:BHP:100:2.34")
        
if __name__ == "__main__":
    unittest.main()