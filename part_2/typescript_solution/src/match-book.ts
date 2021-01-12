import { Book, Order, Trade } from './types';

export function matchBook(
  [side, otherSide]: Book,
  order: Order,
  isBuy: boolean
): Trade[] {
  const trades = [];

  while (!side.empty()) {
    const order2 = side.peek();
    if (
      order.size > 0 &&
      ((isBuy && order.price >= order2.price) ||
        (!isBuy && order.price <= order2.price))
    ) {
      const size = Math.min(order2.size, order.size);
      const price = order2.price;
      const [buyer, seller] = isBuy
        ? [order.owner, order2.owner]
        : [order2.owner, order.owner];

      trades.push({ buyer, seller, size, price });

      order.size -= size;
      order2.size -= size;
      if (order2.size === 0) {
        side.pop();
      }
    } else {
      break;
    }
  }

  if (order.size > 0) {
    otherSide.insert(order);
  }
  return trades;
}
