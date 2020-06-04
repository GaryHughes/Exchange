use std::cmp::{Ordering, PartialEq, PartialOrd};
use std::fmt;

#[derive(Default, Clone, Copy, Debug)]
pub struct Price(pub f32);

impl Eq for Price {}
impl PartialEq for Price {
    fn eq(&self, other: &Self) -> bool {
        other.0 == self.0
    }
}

impl PartialOrd for Price {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        self.0.partial_cmp(&other.0)
    }
}

impl Ord for Price {
    fn cmp(&self, other: &Self) -> Ordering {
        self.partial_cmp(other).unwrap()
    }
}

impl fmt::Display for Price {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_eq() {
        assert!(Price(120.0) == Price(120.0))
    }

    #[test]
    fn test_ord() {
        assert!(Price(120.0) < Price(130.0));
        assert!(Price(130.0) > Price(120.0));

        assert!(Price(120.0) <= Price(130.0));
        assert!(Price(130.0) >= Price(120.0));

        assert!(Price(120.0) <= Price(120.0));
        assert!(Price(120.0) >= Price(120.0));
    }
}
