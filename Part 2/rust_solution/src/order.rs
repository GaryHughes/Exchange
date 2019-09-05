use crate::types::Price;
use std::cmp::{Ordering, PartialOrd};
use std::error::Error;

#[derive(Default, Debug, Eq, PartialEq)]
pub struct Buy {
    pub gen: usize,
    pub participant: String,
    pub quantity: i64,
    pub remaining: i64,
    pub price: Price,
}

#[derive(Default, Debug, Eq, PartialEq)]
pub struct Sell {
    pub gen: usize,
    pub participant: String,
    pub quantity: i64,
    pub remaining: i64,
    pub price: Price,
}

pub enum Order {
    Buy(Buy),
    Sell(Sell),
}

pub fn read(
    participant: &str,
    quantity: &str,
    price: &str,
    gen: usize,
) -> Result<Order, Box<dyn Error>> {
    let qty: i64 = quantity.parse()?;
    let aqty = if qty > 0 { qty } else { -qty };
    let o = if qty > 0 {
        Order::Buy(Buy {
            participant: participant.to_string(),
            quantity: aqty,
            remaining: aqty,
            price: Price(price.parse()?),
            gen,
        })
    } else {
        Order::Sell(Sell {
            participant: participant.to_string(),
            quantity: aqty,
            remaining: aqty,
            price: Price(price.parse()?),
            gen,
        })
    };
    Ok(o)
}

impl PartialOrd for Buy {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        self.price
            .partial_cmp(&other.price)
            .map(|o| o.then(other.gen.cmp(&self.gen)))
    }
}

impl Ord for Buy {
    fn cmp(&self, other: &Self) -> Ordering {
        self.partial_cmp(other).unwrap()
    }
}

impl PartialOrd for Sell {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        other
            .price
            .partial_cmp(&self.price)
            .map(|o| o.then(other.gen.cmp(&self.gen)))
    }
}

impl Ord for Sell {
    fn cmp(&self, other: &Self) -> Ordering {
        self.partial_cmp(other).unwrap()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_read_buy() {
        if let Order::Buy(o) = read("F", "123", "87.125", 99).unwrap() {
            assert_eq!("F", o.participant);
            assert_eq!(123, o.quantity);
            assert_eq!(123, o.remaining);
            assert_eq!(Price(87.125), o.price);
            assert_eq!(99, o.gen)
        } else {
            assert!(false)
        }
    }

    #[test]
    fn test_read_sell() {
        if let Order::Sell(o) = read("F", "-123", "87.125", 99).unwrap() {
            assert_eq!("F", o.participant);
            assert_eq!(123, o.quantity);
            assert_eq!(123, o.remaining);
            assert_eq!(Price(87.125), o.price);
            assert_eq!(99, o.gen)
        } else {
            assert!(false)
        }
    }

    fn buy(px: f32, g: usize) -> Buy {
        Buy {
            gen: g,
            participant: "".to_string(),
            quantity: 0,
            remaining: 0,
            price: Price(px),
        }
    }

    fn sell(px: f32, g: usize) -> Sell {
        Sell {
            gen: g,
            participant: "".to_string(),
            quantity: 0,
            remaining: 0,
            price: Price(px),
        }
    }

    #[test]
    fn test_buy_order_price_priority() {
        // time priority when prices are equal (earlier time > later time)
        assert_eq!(
            Some(Ordering::Greater),
            buy(10.0, 100).partial_cmp(&buy(10.0, 101))
        );
        // price priority when prices are inequal (higher price > lower price)
        assert_eq!(
            Some(Ordering::Greater),
            buy(11.0, 100).partial_cmp(&buy(10.0, 101))
        );
    }

    #[test]
    fn test_sell_order_price_priority() {
        // time priority when prices are equal (earlier time > later time)
        assert_eq!(
            Some(Ordering::Greater),
            sell(10.0, 100).partial_cmp(&sell(10.0, 101))
        );
        // price priority when prices are inequal (higher price > lower price)
        assert_eq!(
            Some(Ordering::Greater),
            sell(10.0, 100).partial_cmp(&sell(11.0, 101))
        );
    }
}
