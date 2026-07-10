use crate::types::Price;
use std::cell::RefCell;
use std::cmp::{Ordering, PartialOrd};
use std::collections::HashMap;
use std::error::Error;
use std::rc::Rc;

#[derive(Debug, Eq, PartialEq)]
pub struct Buy {
    pub gen: usize,
    pub participant: Rc<str>,
    pub quantity: i64,
    pub remaining: i64,
    pub price: Price,
}

#[derive(Debug, Eq, PartialEq)]
pub struct Sell {
    pub gen: usize,
    pub participant: Rc<str>,
    pub quantity: i64,
    pub remaining: i64,
    pub price: Price,
}

pub enum Order {
    Buy(Buy),
    Sell(Sell),
}

thread_local! {
    // Participant ids repeat heavily across an order file, and Rust's String has no small-string
    // optimisation, so interning avoids a fresh heap allocation for every single incoming order.
    static PARTICIPANTS: RefCell<HashMap<Box<str>, Rc<str>>> = RefCell::new(HashMap::new());
}

fn intern(participant: &str) -> Rc<str> {
    PARTICIPANTS.with(|cache| {
        let mut cache = cache.borrow_mut();
        if let Some(existing) = cache.get(participant) {
            return existing.clone();
        }
        let interned: Rc<str> = Rc::from(participant);
        cache.insert(Box::from(participant), interned.clone());
        interned
    })
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
            participant: intern(participant),
            quantity: aqty,
            remaining: aqty,
            price: Price(price.parse()?),
            gen,
        })
    } else {
        Order::Sell(Sell {
            participant: intern(participant),
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
            assert_eq!("F", o.participant.as_ref());
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
            assert_eq!("F", o.participant.as_ref());
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
            participant: Rc::from(""),
            quantity: 0,
            remaining: 0,
            price: Price(px),
        }
    }

    fn sell(px: f32, g: usize) -> Sell {
        Sell {
            gen: g,
            participant: Rc::from(""),
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
