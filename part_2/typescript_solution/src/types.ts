import Heap from 'heap';

export interface Order {
  owner: string;
  size: number;
  price: number;
  time: number;
}

export interface Trade {
  buyer: string;
  seller: string;
  size: number;
  price: number;
}

export type Book = [Heap<Order>, Heap<Order>];

export type Exchange = Map<string, Book>;
