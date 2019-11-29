module Exchange

open System
open System.Collections.Generic

type Order = 
    {   Participant:string
        Instrument:string
        Quantity:Int64
        RemainingQuantity:int64
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


let tokeniseOrderText text =
    if String.IsNullOrEmpty(text) then
        Error "The order text is empty"
    else
        Ok (text.Split(':'))

let validateNumberOfFields (fields:string[]) =
    if fields.Length <> 4 then
        Error (sprintf "Not enough fields, expected 4 got %i" fields.Length)
    else    
        Ok fields

let mutable generation = 0

let createOrder (fields:string[]) =
    generation <- generation + 1
    Ok { Participant = fields.[0]
         Instrument = fields.[1]
         Quantity = Int64.Parse(fields.[2])
         RemainingQuantity = 0L
         Price = Decimal.Parse(fields.[3])
         Generation = generation }

let parseOrder text =
    text
    |> tokeniseOrderText
    |> Result.bind validateNumberOfFields
    |> Result.bind createOrder
        

let orderBooks = new Dictionary<string, OrderBook>()

let getOrderBook (instrument:string) =
    let exists, orderBook = orderBooks.TryGetValue(instrument)
    match exists with
    | true -> orderBook
    | false -> 
        let orderBook = { BuyOrders = new List<Order>(); SellOrders = new List<Order>() }
        orderBooks.Add(instrument, orderBook)
        orderBook        

let insertOrder (order:Order) =
    let orderBook = getOrderBook order.Instrument
    let side, (predicate:Order->bool) =
        match order.Quantity with
        | quantity when quantity < 0L -> 
            (orderBook.SellOrders, 
                fun existing -> 
                    if existing.Price = order.Price then
                       existing.Generation > order.Generation
                    else
                        existing.Price > order.Price
            )
        | _ -> (orderBook.BuyOrders, 
                fun existing -> 
                    if existing.Price = order.Price then
                        existing.Generation > order.Generation
                    else
                        existing.Price < order.Price
               )
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

let trimOrderBook orderBook trade =
    let buyOrder = orderBook.BuyOrders |> Seq.head
    let sellOrder = orderBook.SellOrders |> Seq.head
    let buyOrder = { buyOrder with Quantity = buyOrder.Quantity - trade.Quantity }
    let sellOrder = { sellOrder with Quantity = sellOrder.Quantity + trade.Quantity }
    orderBook.BuyOrders.RemoveAt 0
    orderBook.SellOrders.RemoveAt 0
    if Math.Abs(buyOrder.Quantity) <> 0L then
        orderBook.BuyOrders.Insert(0, buyOrder)
    if Math.Abs(sellOrder.Quantity) <> 0L then
        orderBook.SellOrders.Insert(0, sellOrder)
    trade
  
let matchOrders orderBook =
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
    printfn "%s:%s:%s:%i:%g" trade.Buyer trade.Seller trade.Instrument trade.Quantity trade.Price
   
let printError (message:string) =
    eprintf "%s" message
  
[<EntryPoint>]
let main argv =
    seq {
        while true do
            yield Console.ReadLine()
    }
    |> Seq.takeWhile(String.IsNullOrEmpty >> not)
    |> Seq.map(fun result -> 
        match parseOrder result with 
        | Ok res-> Some res 
        | Error message -> printError message; None )
    |> Seq.choose id
    |> Seq.collect(insertOrder >> matchOrders)
    |> Seq.iter(printTrade)

    0 // return an integer exit code

