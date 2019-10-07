use crate::order::Order;
use crate::order_book::OrderBook;
use crate::trade::Trade;

use std::collections::HashMap;

#[derive(Default)]
pub struct Exchange {
    books: HashMap<String, OrderBook>,
}

impl Exchange {
    pub fn new() -> Self {
        Self {
            books: HashMap::new(),
        }
    }

    pub fn execute(&mut self, instrument: &str, order: Order) -> Vec<Trade> {
        let b = self.book(&instrument);
        b.insert(order);
        b.resolve_matches(&instrument)
    }

    fn book(&mut self, instrument: &str) -> &mut OrderBook {
        // nicer but slower due to more frequent instrument.to_string():
        // self.books
        //     .entry(instrument.to_string())
        //     .or_insert_with(OrderBook::new)
        if !self.books.contains_key(instrument) {
            self.books.insert(instrument.to_string(), OrderBook::new());
        }
        return self.books.get_mut(instrument).unwrap();
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::order;

    #[test]
    fn test_execute_routing() {
        let mut ex = Exchange::new();
        ex.execute("OWLBAT", order::read("F", "123", "10", 100).unwrap());
        ex.execute("COWBEL", order::read("F", "-123", "10", 100).unwrap());
        let bb = ex.book("OWLBAT");
        assert_eq!(1, bb.buys.len());
        let sb = ex.book("COWBEL");
        assert_eq!(1, sb.sells.len())
    }
}
