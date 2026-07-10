import { processor } from './processor';

const exchange = new Map();
let time = 0;

// process.stdout writes are synchronous on POSIX when redirected to a regular file
// (as this benchmark does), so one console.log per trade means one blocking write()
// syscall per trade. Batch trade lines and flush in bulk instead.
const OUTPUT_BATCH_SIZE = 10000;
let outputLines: string[] = [];

function writeTrade(trade: string) {
  outputLines.push(trade);
  if (outputLines.length >= OUTPUT_BATCH_SIZE) {
    process.stdout.write(outputLines.join('\n') + '\n');
    outputLines = [];
  }
}

function flushOutput() {
  if (outputLines.length > 0) {
    process.stdout.write(outputLines.join('\n') + '\n');
    outputLines = [];
  }
}

function handleLine(line: string) {
  processor(line)({ exchange, time: () => ++time }).forEach(writeTrade);
}

// readline's 'line' event goes through the EventEmitter machinery per line, which is
// significant overhead at 10M-line scale. Read stdin in raw chunks instead and split
// on '\n' within a manually maintained buffer.
let leftover = '';

process.stdin.setEncoding('utf8');

process.stdin.on('data', (chunk: string) => {
  leftover += chunk;
  let start = 0;
  let newlineIndex;
  while ((newlineIndex = leftover.indexOf('\n', start)) !== -1) {
    let line = leftover.slice(start, newlineIndex);
    if (line.endsWith('\r')) {
      line = line.slice(0, -1);
    }
    start = newlineIndex + 1;
    handleLine(line);
  }
  leftover = leftover.slice(start);
});

process.stdin.on('end', () => {
  if (leftover.length > 0) {
    let line = leftover;
    if (line.endsWith('\r')) {
      line = line.slice(0, -1);
    }
    handleLine(line);
  }
  flushOutput();
});
