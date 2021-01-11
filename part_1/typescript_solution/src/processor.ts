import { flow, pipe } from 'fp-ts/lib/function';
import { matchMarket } from './match-market';
import { Trade } from './types';
import * as A from 'fp-ts/Array';
import * as R from 'fp-ts/Reader';

function parse(line: string) {
  const [owner, instrument, size, price] = line.split(':');

  const size2 = Number(size);
  const isBuy = size2 > 0;
  const order = { owner, size: Math.abs(size2), price: Number(price) };

  return { instrument, order, isBuy };
}

function stringify(instrument: string, { buyer, seller, price, size }: Trade) {
  return `${buyer}:${seller}:${instrument}:${size}:${price}`;
}

export const processor = flow(parse, (parsed) =>
  pipe(
    matchMarket(parsed),
    R.map(A.map((trade) => stringify(parsed.instrument, trade)))
  )
);
