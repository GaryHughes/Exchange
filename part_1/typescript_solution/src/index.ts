import * as readline from 'readline';
import { processor } from './processor';

const rl = readline.createInterface({
  input: process.stdin,
});

const exchange = new Map();

rl.on('line', (line) => {
  processor(line)(exchange).forEach((trade) => console.log(trade));
});
