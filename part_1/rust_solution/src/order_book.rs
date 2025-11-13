use crate::order::Order;
use crate::trade::Trade;
use std::cmp::min;

pub struct OrderBook {
    pub buys: Vec<Order>,
    pub sells: Vec<Order>,
}

impl OrderBook {
    pub fn new() -> OrderBook {
        return OrderBook {
            buys: vec![],
            sells: vec![],
        };
    }

    pub fn insert(&mut self, order: Order) {
        if order.is_buy {
            self.buys.push(order);
            // invariant: top bid is the last element
            self.buys.sort_unstable_by(|a, b| {
                if a.price == b.price {
                    b.gen.cmp(&a.gen)
                } else {
                    a.price.partial_cmp(&b.price).unwrap()
                }
            })
        } else {
            self.sells.push(order);
            // invariant: top sell is the last element
            self.sells.sort_unstable_by(|a, b| {
                if a.price == b.price {
                    b.gen.cmp(&a.gen)
                } else {
                    b.price.partial_cmp(&a.price).unwrap()
                }
            })
        }
    }

    pub fn resolve_matches(&mut self) -> Vec<Trade> {
        let mut trades = Vec::<Trade>::new();
        while !self.buys.is_empty() && !self.sells.is_empty() {
            let buy = &mut self.buys.last_mut().unwrap();
            let sell = &mut self.sells.last_mut().unwrap();
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
                instrument: buy.instrument.to_string(),
                quantity: match_qty,
                price: match_price,
                bgen: buy.gen,
                sgen: sell.gen,
            };
            trades.push(t);
            if buy.remaining == match_qty {
                self.buys.pop();
            } else {
                buy.remaining -= match_qty;
            }
            if sell.remaining == match_qty {
                self.sells.pop();
            } else {
                sell.remaining -= match_qty;
            }
        }
        trades
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_insert_buy() {
        let o = Order::read("F", "OWLBAT", "123", "87.125", 100).unwrap();
        let mut ob = OrderBook::new();
        ob.insert(o);
        assert_eq!(1, ob.buys.len());
        assert_eq!(0, ob.sells.len())
    }

    #[test]
    fn test_insert_sell() {
        let o = Order::read("F", "OWLBAT", "-123", "87.125", 100).unwrap();
        let mut ob = OrderBook::new();
        ob.insert(o);
        assert_eq!(0, ob.buys.len());
        assert_eq!(1, ob.sells.len())
    }

    #[test]
    fn test_buy_order_price_priority() {
        let mut ob = OrderBook::new();
        ob.insert(Order::read("F", "OWLBAT", "123", "11", 100).unwrap());
        ob.insert(Order::read("F", "OWLBAT", "123", "12", 101).unwrap());
        ob.insert(Order::read("F", "OWLBAT", "123", "10", 102).unwrap());
        assert_eq!(12.0, ob.buys[2].price);
        assert_eq!(11.0, ob.buys[1].price);
        assert_eq!(10.0, ob.buys[0].price)
    }

    #[test]
    fn test_buy_order_time_priority() {
        let mut ob = OrderBook::new();
        ob.insert(Order::read("F", "OWLBAT", "123", "10", 102).unwrap());
        ob.insert(Order::read("F", "OWLBAT", "123", "10", 100).unwrap());
        ob.insert(Order::read("F", "OWLBAT", "123", "10", 101).unwrap());
        assert_eq!(100, ob.buys[2].gen);
        assert_eq!(101, ob.buys[1].gen);
        assert_eq!(102, ob.buys[0].gen)
    }

    #[test]
    fn test_sell_order_price_priority() {
        let mut ob = OrderBook::new();
        ob.insert(Order::read("F", "OWLBAT", "-123", "11", 100).unwrap());
        ob.insert(Order::read("F", "OWLBAT", "-123", "12", 101).unwrap());
        ob.insert(Order::read("F", "OWLBAT", "-123", "10", 102).unwrap());
        assert_eq!(10.0, ob.sells[2].price);
        assert_eq!(11.0, ob.sells[1].price);
        assert_eq!(12.0, ob.sells[0].price)
    }

    #[test]
    fn test_sell_order_time_priority() {
        let mut ob = OrderBook::new();
        ob.insert(Order::read("F", "OWLBAT", "-123", "10", 102).unwrap());
        ob.insert(Order::read("F", "OWLBAT", "-123", "10", 100).unwrap());
        ob.insert(Order::read("F", "OWLBAT", "-123", "10", 101).unwrap());
        assert_eq!(100, ob.sells[2].gen);
        assert_eq!(101, ob.sells[1].gen);
        assert_eq!(102, ob.sells[0].gen)
    }

    #[test]
    fn test_resolve_matches_stops_when_no_orders_remain() {
        let mut ob = OrderBook::new();
        let trades = ob.resolve_matches();
        assert!(trades.is_empty())
    }

    #[test]
    fn test_resolve_matches_stops_when_no_buys_remain() {
        let mut ob = OrderBook::new();
        ob.insert(Order::read("F", "OWLBAT", "-123", "10", 100).unwrap());
        let trades = ob.resolve_matches();
        assert!(trades.is_empty())
    }

    #[test]
    fn test_resolve_matches_stops_when_no_sells_remain() {
        let mut ob = OrderBook::new();
        ob.insert(Order::read("F", "OWLBAT", "123", "10", 100).unwrap());
        let trades = ob.resolve_matches();
        assert!(trades.is_empty())
    }

    #[test]
    fn test_resolve_matches_stops_when_not_overlapping() {
        let mut ob = OrderBook::new();
        ob.insert(Order::read("F", "OWLBAT", "123", "10", 100).unwrap());
        ob.insert(Order::read("F", "OWLBAT", "-123", "11", 101).unwrap());
        let trades = ob.resolve_matches();
        assert!(trades.is_empty())
    }

    fn order(p: &str, i: &str, qty: i64, rem: i64, px: f32, is_buy: bool, g: usize) -> Order {
        Order {
            gen: g,
            participant: p.to_string(),
            instrument: i.to_string(),
            quantity: qty,
            remaining: rem,
            price: px,
            is_buy: is_buy,
        }
    }

    fn trade(b: &str, s: &str, i: &str, q: i64, p: f32, bgen: usize, sgen: usize) -> Trade {
        Trade {
            buyer: b.to_string(),
            seller: s.to_string(),
            instrument: i.to_string(),
            quantity: q,
            price: p,
            bgen: bgen,
            sgen: sgen,
        }
    }

    #[test]
    fn test_resolve_new_sell_filled() {
        let mut ob = OrderBook::new();
        ob.insert(order("B1", "OWLBAT", 50, 50, 10.0, true, 100));
        ob.insert(order("B2", "OWLBAT", 100, 100, 10.0, true, 101));
        ob.insert(order("S", "OWLBAT", 75, 75, 9.0, false, 102));
        let trades = ob.resolve_matches();
        assert_eq!(2, trades.len());
        assert_eq!(trade("B1", "S", "OWLBAT", 50, 10.0, 100, 102), trades[0]);
        assert_eq!(trade("B2", "S", "OWLBAT", 25, 10.0, 101, 102), trades[1]);
        assert!(ob.sells.is_empty());
        assert_eq!(1, ob.buys.len());
        assert_eq!(order("B2", "OWLBAT", 100, 75, 10.0, true, 101), ob.buys[0]);
    }

    #[test]
    fn test_resolve_new_buy_filled() {
        let mut ob = OrderBook::new();
        ob.insert(order("S1", "OWLBAT", 50, 50, 9.0, false, 100));
        ob.insert(order("S2", "OWLBAT", 100, 100, 9.0, false, 101));
        ob.insert(order("B", "OWLBAT", 75, 75, 10.0, true, 102));
        let trades = ob.resolve_matches();
        assert_eq!(2, trades.len());
        assert_eq!(trade("B", "S1", "OWLBAT", 50, 9.0, 102, 100), trades[0]);
        assert_eq!(trade("B", "S2", "OWLBAT", 25, 9.0, 102, 101), trades[1]);
        assert!(ob.buys.is_empty());
        assert_eq!(1, ob.sells.len());
        assert_eq!(order("S2", "OWLBAT", 100, 75, 9.0, false, 101), ob.sells[0]);
    }

    #[test]
    fn test_resolve_new_sell_partial() {
        let mut ob = OrderBook::new();
        ob.insert(order("B1", "OWLBAT", 50, 50, 10.0, true, 100));
        ob.insert(order("B2", "OWLBAT", 100, 100, 10.0, true, 101));
        ob.insert(order("S", "OWLBAT", 200, 200, 9.0, false, 102));
        let trades = ob.resolve_matches();
        assert_eq!(2, trades.len());
        assert_eq!(trade("B1", "S", "OWLBAT", 50, 10.0, 100, 102), trades[0]);
        assert_eq!(trade("B2", "S", "OWLBAT", 100, 10.0, 101, 102), trades[1]);
        assert!(ob.buys.is_empty());
        assert_eq!(1, ob.sells.len());
        assert_eq!(order("S", "OWLBAT", 200, 50, 9.0, false, 102), ob.sells[0]);
    }

    #[test]
    fn test_resolve_new_buy_partial() {
        let mut ob = OrderBook::new();
        ob.insert(order("S1", "OWLBAT", 50, 50, 9.0, false, 100));
        ob.insert(order("S2", "OWLBAT", 100, 100, 9.0, false, 101));
        ob.insert(order("B", "OWLBAT", 200, 200, 10.0, true, 102));
        let trades = ob.resolve_matches();
        assert_eq!(2, trades.len());
        assert_eq!(trade("B", "S1", "OWLBAT", 50, 9.0, 102, 100), trades[0]);
        assert_eq!(trade("B", "S2", "OWLBAT", 100, 9.0, 102, 101), trades[1]);
        assert!(ob.sells.is_empty());
        assert_eq!(1, ob.buys.len());
        assert_eq!(order("B", "OWLBAT", 200, 50, 10.0, true, 102), ob.buys[0]);
    }
}
