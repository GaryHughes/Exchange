#! /usr/bin/env python

import unittest
import Exchange

class TestOrder(unittest.TestCase):
    def test_create(self):
        o = Exchange.Order("A:AUDUSD:100:1.47\n")
        self.assertEqual(o.qty, 100)
        self.assertEqual(o.instrument, "AUDUSD")
        self.assertEqual(o.id, "A")
        self.assertEqual(o.price, 1.47)
        self.assertEqual(o.side, "B")

    def test_create_sell(self):
        o = Exchange.Order("A:AUDUSD:-100:1.47\n")
        self.assertEqual(o.qty, 100)
        self.assertEqual(o.instrument, "AUDUSD")
        self.assertEqual(o.id, "A")
        self.assertEqual(o.price, 1.47)
        self.assertEqual(o.side, "S")

    def test_generation(self):
        o = Exchange.Order("A:AUDUSD:-100:1.47\n")
        o2 = Exchange.Order("A:AUDUSD:-100:1.47\n")
        self.assertGreater(o2.generation, o.generation)
        
if __name__ == "__main__":
    unittest.main()