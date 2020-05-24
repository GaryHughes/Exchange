#! /usr/bin/env python3

import unittest
import Exchange

class TestOrder(unittest.TestCase):
    def test_create(self):
        o = Exchange.Order("A", 100, 1.47)
        self.assertEqual(o.qty, 100)
        self.assertEqual(o.id, "A")
        self.assertEqual(o.price, 1.47)
        self.assertEqual(o.side, "B")

    def test_create_sell(self):
        o = Exchange.Order("A", -100, 1.47)
        self.assertEqual(o.qty, 100)
        self.assertEqual(o.id, "A")
        self.assertEqual(o.price, 1.47)
        self.assertEqual(o.side, "S")
       
if __name__ == "__main__":
    unittest.main()