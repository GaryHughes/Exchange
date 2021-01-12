import { matchBook } from './match-book';
import { Exchange, Order, Trade } from './types';
import Heap from 'heap';

export function matchExchange({
  instrument,
  order,
  isBuy,
}: {
  instrument: string;
  order: Order;
  isBuy: boolean;
}) {
  return ({ exchange }: { exchange: Exchange }): Trade[] => {
    let book = exchange.get(instrument);
    if (!book) {
      book = [
        // Buy side - higher price better
        new Heap((a, b) => {
          const cmp = b.price - a.price;
          if (cmp === 0) {
            return a.time - b.time;
          }
          return cmp;
        }),
        // Sell side - lower price better
        new Heap((a, b) => {
          const cmp = a.price - b.price;
          if (cmp === 0) {
            return a.time - b.time;
          }
          return cmp;
        }),
      ];
      exchange.set(instrument, book);
    }

    const [side, otherSide] = isBuy ? [book[1], book[0]] : book;

    return matchBook([side, otherSide], order, isBuy);
  };
}
