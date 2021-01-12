import * as readline from 'readline';
import { processor } from './processor';

const rl = readline.createInterface({
  input: process.stdin,
});

const exchange = new Map();
let time = 0;

rl.on('line', (line) => {
  processor(line)({ exchange, time: () => ++time }).forEach((trade) =>
    console.log(trade)
  );
});
