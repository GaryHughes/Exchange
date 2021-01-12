## TypeScript solution (Part 2)

This solution is functions only, no classes. We depend on two external libs at
runtime - fp-ts and heap. Our buy/sell queues are thus binary heaps this time, 
which is vastly more performant that the old array slicing we were doing.

## Build tools

Install NPM and NodeJS v14 or above using [nvm](https://github.com/nvm-sh/nvm#installing-and-updating)

## Building and running the TypeScript solution

```
% npm install
% npm run build
% npm run test
```