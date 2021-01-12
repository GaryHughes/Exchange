export interface Order {
  owner: string;
  size: number;
  price: number;
}

export interface Trade {
  buyer: string;
  seller: string;
  size: number;
  price: number;
}

export type Book = [Order[], Order[]];

export type Exchange = Map<string, Book>;
