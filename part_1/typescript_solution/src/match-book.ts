import { Book, Order, Trade } from './types';

function insert(side: Order[], order: Order, isBuy: boolean) {
  let min = 0;
  let max = side.length;
  while (min < max) {
    const index = min + Math.floor((max - min) / 2);
    const order2 = side[index];
    if (
      (isBuy && order.price > order2.price) ||
      (!isBuy && order.price < order2.price)
    ) {
      max = index;
    } else {
      min = index + 1;
    }
  }
  side.splice(min, 0, order);
}

export function matchBook(
  [side, otherSide]: Book,
  order: Order,
  isBuy: boolean
): Trade[] {
  const trades = [];

  let done = 0;
  for (const order2 of side) {
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
        ++done;
      }
    } else {
      break;
    }
  }

  side.splice(0, done);
  if (order.size > 0) {
    insert(otherSide, order, isBuy);
  }
  return trades;
}
