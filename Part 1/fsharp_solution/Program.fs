open System
open System.Collections.Generic

type Order =
    {   Participant:string
        Instrument:string
        Quantity:Int64
        RemainingQuantity:int
        Price:decimal
        Generation:int }

type Trade =
    {   Buyer:string
        Seller:string
        Instrument:string
        Quantity:Int64
        Price:decimal }

type OrderBook =
    {   BuyOrders : List<Order>
        SellOrders : List<Order>    }

let parseOrder (orderText:string) =
    let components = orderText.Split(':')
    {   Participant = components.[0]
        Instrument = components.[1]
        Quantity = Int64.Parse(components.[2])
        RemainingQuantity = 0
        Price = Decimal.Parse(components.[3])
        Generation = Environment.TickCount }

let orderBooks = new Dictionary<string, OrderBook>()

let getOrderBook (instrument:string) =
    let exists, orderBook = orderBooks.TryGetValue(instrument)
    match exists with
    | true -> orderBook
    | false -> 
        let orderBook = { BuyOrders = new List<Order>(); SellOrders = new List<Order>() }
        orderBooks.Add(instrument, orderBook)
        orderBook        

let insertOrder (order:Order) : OrderBook =
    let orderBook = getOrderBook order.Instrument
    let side, (predicate:Order->bool) =
        match order.Quantity with
        | quantity when quantity < 0L -> (orderBook.SellOrders, fun other -> order.Price < other.Price)
        | _ -> (orderBook.BuyOrders, fun other -> order.Price > other.Price)
    match side |> Seq.tryFindIndex(predicate) with
    | Some index -> side.Insert(index, order) 
    | None -> side.Add(order)
    orderBook

let matchingOrders orderBook =
    if orderBook.BuyOrders |> Seq.isEmpty || orderBook.SellOrders |> Seq.isEmpty then
        None
    else
        let buyOrder = orderBook.BuyOrders |> Seq.head
        let sellOrder = orderBook.SellOrders |> Seq.head
        if buyOrder.Price >= sellOrder.Price then
            Some (buyOrder, sellOrder)
        else
            None     

let createTrade (buyOrder:Order,  sellOrder:Order) =
    let tradeQuantity = [|Math.Abs(buyOrder.Quantity); Math.Abs(sellOrder.Quantity)|] |> Seq.min
    let tradePrice = if buyOrder.Generation < sellOrder.Generation then buyOrder.Price else sellOrder.Price
    {   Buyer = buyOrder.Participant
        Seller = sellOrder.Participant
        Instrument = buyOrder.Instrument
        Price = tradePrice
        Quantity = tradeQuantity }

let trimOrderBook (orderBook:OrderBook) trade =
    let buyOrder = orderBook.BuyOrders |> Seq.head
    let sellOrder = orderBook.SellOrders |> Seq.head
    let buyOrder = { buyOrder with Quantity = buyOrder.Quantity - trade.Quantity }
    let sellOrder = { sellOrder with Quantity = sellOrder.Quantity + trade.Quantity }
    orderBook.BuyOrders.RemoveAt 0
    orderBook.SellOrders.RemoveAt 0
    if buyOrder.Quantity > 0L then
        orderBook.BuyOrders.Insert(0, buyOrder)
    if sellOrder.Quantity > 0L then
        orderBook.SellOrders.Insert(0, sellOrder)
    trade
  
let matchOrders orderBook : Trade list =
    let trimOrderBook = trimOrderBook orderBook
    seq {
        while true do
            yield matchingOrders orderBook
    }
    |> Seq.takeWhile Option.isSome
    |> Seq.choose id
    |> Seq.map(createTrade >> trimOrderBook)
    |> List.ofSeq

let printTrade trade =
    printfn "%s:%s:%s:%i:%f" trade.Buyer trade.Seller trade.Instrument trade.Quantity trade.Price
   
  
[<EntryPoint>]
let main argv =
    seq {
        while true do
            yield Console.ReadLine()
    }
    |> Seq.takeWhile(String.IsNullOrEmpty >> not) 
    |> Seq.collect(parseOrder >> insertOrder >> matchOrders)
    |> Seq.iter(printTrade)

    0 // return an integer exit code
