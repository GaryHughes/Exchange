## TypeScript solution (Part 1)

This solution is functions only, no classes. The only external lib we depend on at 
runtime is fp-ts, which is entirely unnecessary but fun to use! Our buy/sell queues
are simple arrays, with our inserts being done using splice and a binary seach to 
find the correct ordered insertion point.

## Build tools

Install NPM and NodeJS v14 or above using [nvm](https://github.com/nvm-sh/nvm#installing-and-updating)

## Building and running the TypeScript solution

```
% npm install
% npm run build
% npm run test
```