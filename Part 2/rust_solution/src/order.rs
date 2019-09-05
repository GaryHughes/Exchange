use crate::types::Price;
use std::cmp::{Ordering, PartialOrd};
use std::error::Error;

#[derive(Default, Debug, Eq, PartialEq)]
pub struct Order {
    pub gen: usize,
    pub participant: String,
    pub instrument: String,
    pub quantity: i64,
    pub remaining: i64,
    pub price: Price,
    pub is_buy: bool,
}

impl Order {
    pub fn read(
        participant: &str,
        instrument: &str,
        quantity: &str,
        price: &str,
        gen: usize,
    ) -> Result<Order, Box<dyn Error>> {
        let qty: i64 = quantity.parse()?;
        let aqty = if qty > 0 { qty } else { -qty };
        let order = Order {
            participant: participant.to_string(),
            instrument: instrument.to_string(),
            quantity: aqty,
            remaining: aqty,
            is_buy: qty > 0,
            price: Price(price.parse()?),
            gen,
        };
        Ok(order)
    }
}

impl PartialOrd for Order {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        assert!(self.is_buy == other.is_buy);
        if self.is_buy {
            self.price
                .partial_cmp(&other.price)
                .map(|o| o.then(other.gen.cmp(&self.gen)))
        } else {
            other
                .price
                .partial_cmp(&self.price)
                .map(|o| o.then(other.gen.cmp(&self.gen)))
        }
    }
}

impl Ord for Order {
    fn cmp(&self, other: &Self) -> Ordering {
        self.partial_cmp(other).unwrap()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_read() {
        let o = Order::read("F", "OWLBAT", "-123", "87.125", 99).unwrap();
        assert_eq!("F", o.participant);
        assert_eq!("OWLBAT", o.instrument);
        assert_eq!(123, o.quantity);
        assert_eq!(123, o.remaining);
        assert_eq!(Price(87.125), o.price);
        assert_eq!(false, o.is_buy);
        assert_eq!(99, o.gen)
    }

    fn order_pp(px: f32, is_buy: bool, g: usize) -> Order {
        Order {
            gen: g,
            participant: "".to_string(),
            instrument: "".to_string(),
            quantity: 0,
            remaining: 0,
            price: Price(px),
            is_buy,
        }
    }

    #[test]
    fn test_buy_order_price_priority() {
        // time priority when prices are equal (earlier time > later time)
        assert_eq!(
            Some(Ordering::Greater),
            order_pp(10.0, true, 100).partial_cmp(&order_pp(10.0, true, 101))
        );
        // price priority when prices are inequal (higher price > lower price)
        assert_eq!(
            Some(Ordering::Greater),
            order_pp(11.0, true, 100).partial_cmp(&order_pp(10.0, true, 101))
        );
    }

    #[test]
    fn test_sell_order_price_priority() {
        // time priority when prices are equal (earlier time > later time)
        assert_eq!(
            Some(Ordering::Greater),
            order_pp(10.0, false, 100).partial_cmp(&order_pp(10.0, false, 101))
        );
        // price priority when prices are inequal (higher price > lower price)
        assert_eq!(
            Some(Ordering::Greater),
            order_pp(10.0, false, 100).partial_cmp(&order_pp(11.0, false, 101))
        );
    }
}
