use std::error::Error;

#[derive(Debug, PartialEq)]
pub struct Order {
    pub gen: usize,
    pub participant: String,
    pub instrument: String,
    pub quantity: i64,
    pub remaining: i64,
    pub price: f32,
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
            price: price.parse()?,
            gen: gen,
        };
        Ok(order)
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
        assert_eq!(87.125, o.price);
        assert_eq!(false, o.is_buy);
        assert_eq!(99, o.gen)
    }
}
