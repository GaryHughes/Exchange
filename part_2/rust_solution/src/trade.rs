use std::fmt;
use std::rc::Rc;

use crate::types::Price;

#[derive(Debug, PartialEq)]
pub struct Trade {
    pub buyer: Rc<str>,
    pub seller: Rc<str>,
    pub instrument: String,
    pub quantity: i64,
    pub price: Price,
    pub bgen: usize,
    pub sgen: usize,
}

impl fmt::Display for Trade {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "{}:{}:{}:{}:{}",
            self.buyer, self.seller, self.instrument, self.quantity, self.price,
        )
    }
}
