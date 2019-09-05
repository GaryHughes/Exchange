use crate::order::{Order, Buy, Sell};
use crate::trade::Trade;
use std::cmp::min;
use std::collections::BinaryHeap;

#[derive(Default)]
pub struct OrderBook {
    pub buys: BinaryHeap<Buy>,
    pub sells: BinaryHeap<Sell>,
}

impl OrderBook {
    pub fn new() -> Self {
        Self {
            buys: BinaryHeap::new(),
            sells: BinaryHeap::new(),
        }
    }

    pub fn insert(&mut self, order: Order) {
        match order {
            Order::Buy(o) => self.buys.push(o),
            Order::Sell(o) => self.sells.push(o),
        }
    }

    pub fn resolve_matches(&mut self, instrument: &str) -> Vec<Trade> {
        let mut trades = Vec::<Trade>::new();
        while !self.buys.is_empty() && !self.sells.is_empty() {
            let buy = self.buys.peek().unwrap();
            let sell = self.sells.peek().unwrap();
            if buy.price < sell.price {
                break;
            }
            let match_price = if buy.gen < sell.gen {
                buy.price
            } else {
                sell.price
            };
            let match_qty = min(buy.remaining, sell.remaining);
            let t = Trade {
                buyer: buy.participant.to_string(),
                seller: sell.participant.to_string(),
                instrument: instrument.to_string(),
                quantity: match_qty,
                price: match_price,
                bgen: buy.gen,
                sgen: sell.gen,
            };
            trades.push(t);
            if buy.remaining == match_qty {
                self.buys.pop();
            } else {
                let mut buy = self.buys.peek_mut().unwrap();
                buy.remaining -= match_qty;
            }
            if sell.remaining == match_qty {
                self.sells.pop();
            } else {
                let mut sell = self.sells.peek_mut().unwrap();
                sell.remaining -= match_qty;
            }
        }
        trades
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::types::Price;

    fn buy(p: &str, px: f32, qty: i64, rem: i64, g: usize) -> Buy {
        Buy {
            gen: g,
            participant: p.to_string(),
            quantity: qty,
            remaining: rem,
            price: Price(px),
        }
    }

    fn sell(p: &str, px: f32, qty: i64, rem: i64, g: usize) -> Sell {
        Sell {
            gen: g,
            participant: p.to_string(),
            quantity: qty,
            remaining: rem,
            price: Price(px),
        }
    }

    #[test]
    fn test_insert_buy() {
        let o = buy("B", 87.125, 1, 1, 100);
        let mut ob = OrderBook::new();
        ob.insert(Order::Buy(o));
        assert_eq!(1, ob.buys.len());
        assert_eq!(0, ob.sells.len())
    }

    #[test]
    fn test_insert_sell() {
        let o = sell("S", 87.125, 1, 1, 100);
        let mut ob = OrderBook::new();
        ob.insert(Order::Sell(o));
        assert_eq!(0, ob.buys.len());
        assert_eq!(1, ob.sells.len())
    }

    #[test]
    fn test_buy_order_price_priority() {
        let mut ob = OrderBook::new();
        ob.insert(Order::Buy(buy("B", 11.0, 1, 1, 100)));
        ob.insert(Order::Buy(buy("B", 12.0, 1, 1, 101)));
        ob.insert(Order::Buy(buy("B", 10.0, 1, 1, 102)));
        assert_eq!(Price(12.0), ob.buys.pop().unwrap().price);
        assert_eq!(Price(11.0), ob.buys.pop().unwrap().price);
        assert_eq!(Price(10.0), ob.buys.pop().unwrap().price)
    }

    #[test]
    fn test_buy_order_time_priority() {
        let mut ob = OrderBook::new();
        ob.insert(Order::Buy(buy("B", 10.0, 1, 1, 102)));
        ob.insert(Order::Buy(buy("B", 10.0, 1, 1, 100)));
        ob.insert(Order::Buy(buy("B", 10.0, 1, 1, 101)));
        assert_eq!(100, ob.buys.pop().unwrap().gen);
        assert_eq!(101, ob.buys.pop().unwrap().gen);
        assert_eq!(102, ob.buys.pop().unwrap().gen)
    }

    #[test]
    fn test_sell_order_price_priority() {
        let mut ob = OrderBook::new();
        ob.insert(Order::Sell(sell("S", 11.0, 1, 1, 100)));
        ob.insert(Order::Sell(sell("S", 12.0, 1, 1, 101)));
        ob.insert(Order::Sell(sell("S", 10.0, 1, 1, 102)));
        assert_eq!(Price(10.0), ob.sells.pop().unwrap().price);
        assert_eq!(Price(11.0), ob.sells.pop().unwrap().price);
        assert_eq!(Price(12.0), ob.sells.pop().unwrap().price)
    }

    #[test]
    fn test_sell_order_time_priority() {
        let mut ob = OrderBook::new();
        ob.insert(Order::Sell(sell("S", 10.0, 1, 1, 102)));
        ob.insert(Order::Sell(sell("S", 10.0, 1, 1, 100)));
        ob.insert(Order::Sell(sell("S", 10.0, 1, 1, 101)));
        assert_eq!(100, ob.sells.pop().unwrap().gen);
        assert_eq!(101, ob.sells.pop().unwrap().gen);
        assert_eq!(102, ob.sells.pop().unwrap().gen)
    }

    #[test]
    fn test_resolve_matches_stops_when_no_orders_remain() {
        let mut ob = OrderBook::new();
        let trades = ob.resolve_matches("OWLBAT");
        assert!(trades.is_empty())
    }

    #[test]
    fn test_resolve_matches_stops_when_no_buys_remain() {
        let mut ob = OrderBook::new();
        ob.insert(Order::Sell(sell("S", 10.0, 1, 1, 100)));
        let trades = ob.resolve_matches("OWLBAT");
        assert!(trades.is_empty())
    }

    #[test]
    fn test_resolve_matches_stops_when_no_sells_remain() {
        let mut ob = OrderBook::new();
        ob.insert(Order::Buy(buy("B", 10.0, 1, 1, 100)));
        let trades = ob.resolve_matches("OWLBAT");
        assert!(trades.is_empty())
    }

    #[test]
    fn test_resolve_matches_stops_when_not_overlapping() {
        let mut ob = OrderBook::new();
        ob.insert(Order::Buy(buy("B", 10.0, 1, 1, 100)));
        ob.insert(Order::Sell(sell("S", 11.0, 1, 1, 100)));
        let trades = ob.resolve_matches("OWLBAT");
        assert!(trades.is_empty())
    }

    fn trade(b: &str, s: &str, i: &str, q: i64, p: f32, bgen: usize, sgen: usize) -> Trade {
        Trade {
            buyer: b.to_string(),
            seller: s.to_string(),
            instrument: i.to_string(),
            quantity: q,
            price: Price(p),
            bgen,
            sgen,
        }
    }

    #[test]
    fn test_resolve_new_sell_filled() {
        let mut ob = OrderBook::new();
        ob.insert(Order::Buy(buy("B1", 10.0, 50, 50, 100)));
        ob.insert(Order::Buy(buy("B2", 10.0, 100, 100, 101)));
        ob.insert(Order::Sell(sell("S", 9.0, 75, 75, 102)));
        let trades = ob.resolve_matches("OWLBAT");
        assert_eq!(2, trades.len());
        assert_eq!(trade("B1", "S", "OWLBAT", 50, 10.0, 100, 102), trades[0]);
        assert_eq!(trade("B2", "S", "OWLBAT", 25, 10.0, 101, 102), trades[1]);
        assert!(ob.sells.is_empty());
        assert_eq!(1, ob.buys.len());
        assert_eq!(buy("B2", 10.0, 100, 75, 101), ob.buys.pop().unwrap());
    }

    #[test]
    fn test_resolve_new_buy_filled() {
        let mut ob = OrderBook::new();
        ob.insert(Order::Sell(sell("S1", 9.0, 50, 50, 100)));
        ob.insert(Order::Sell(sell("S2", 9.0, 100, 100, 101)));
        ob.insert(Order::Buy(buy("B", 10.0, 75, 75, 102)));
        let trades = ob.resolve_matches("OWLBAT");
        assert_eq!(2, trades.len());
        assert_eq!(trade("B", "S1", "OWLBAT", 50, 9.0, 102, 100), trades[0]);
        assert_eq!(trade("B", "S2", "OWLBAT", 25, 9.0, 102, 101), trades[1]);
        assert!(ob.buys.is_empty());
        assert_eq!(1, ob.sells.len());
        assert_eq!(sell("S2", 9.0, 100, 75, 101), ob.sells.pop().unwrap());
    }

    #[test]
    fn test_resolve_new_sell_partial() {
        let mut ob = OrderBook::new();
        ob.insert(Order::Buy(buy("B1", 10.0, 50, 50, 100)));
        ob.insert(Order::Buy(buy("B2", 10.0, 100, 100, 101)));
        ob.insert(Order::Sell(sell("S", 9.0, 200, 200, 102)));
        let trades = ob.resolve_matches("OWLBAT");
        assert_eq!(2, trades.len());
        assert_eq!(trade("B1", "S", "OWLBAT", 50, 10.0, 100, 102), trades[0]);
        assert_eq!(trade("B2", "S", "OWLBAT", 100, 10.0, 101, 102), trades[1]);
        assert!(ob.buys.is_empty());
        assert_eq!(1, ob.sells.len());
        assert_eq!(sell("S", 9.0, 200, 50, 102), ob.sells.pop().unwrap());
    }

    #[test]
    fn test_resolve_new_buy_partial() {
        let mut ob = OrderBook::new();
        ob.insert(Order::Sell(sell("S1", 9.0, 50, 50, 100)));
        ob.insert(Order::Sell(sell("S2", 9.0, 100, 100, 101)));
        ob.insert(Order::Buy(buy("B", 10.0, 200, 200, 102)));
        let trades = ob.resolve_matches("OWLBAT");
        assert_eq!(2, trades.len());
        assert_eq!(trade("B", "S1", "OWLBAT", 50, 9.0, 102, 100), trades[0]);
        assert_eq!(trade("B", "S2", "OWLBAT", 100, 9.0, 102, 101), trades[1]);
        assert!(ob.sells.is_empty());
        assert_eq!(1, ob.buys.len());
        assert_eq!(buy("B", 10.0, 200, 50, 102), ob.buys.pop().unwrap());
    }
}
