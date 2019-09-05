use std::fmt;

use crate::types::Price;

#[derive(Debug, PartialEq)]
pub struct Trade {
    pub buyer: String,
    pub seller: String,
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
