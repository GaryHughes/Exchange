#! /usr/bin/env python

import unittest
import Exchange

class TestOrder(unittest.TestCase):
    def test_create(self):
        o = Exchange.BuyOrder("A", 100, 1.47)
        self.assertEqual(o.qty, 100)
        self.assertEqual(o.id, "A")
        self.assertEqual(o.price, 1.47)

    def test_create_sell(self):
        o = Exchange.SellOrder("A", 100, 1.47)
        self.assertEqual(o.qty, 100)
        self.assertEqual(o.id, "A")
        self.assertEqual(o.price, 1.47)
        
if __name__ == "__main__":
    unittest.main()
