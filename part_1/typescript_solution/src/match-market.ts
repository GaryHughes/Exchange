import { matchBook } from './match-book';
import { Exchange, Order, Trade } from './types';

export function matchMarket({
  instrument,
  order,
  isBuy,
}: {
  instrument: string;
  order: Order;
  isBuy: boolean;
}) {
  return (exchange: Exchange): Trade[] => {
    let book = exchange.get(instrument);
    if (!book) {
      book = [[], []];
      exchange.set(instrument, book);
    }

    const [side, otherSide] = isBuy ? [book[1], book[0]] : book;

    return matchBook([side, otherSide], order, isBuy);
  };
}
