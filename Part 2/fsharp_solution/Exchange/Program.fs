module Exchange

open System
open System.IO
open System.Text
open System.Collections.Generic
open FSharp.Data
open C5

type Order =
    {   Participant:string
        Instrument:string
        Quantity:Int64
        RemainingQuantity:int
        Price:decimal
        Generation:int }
    
type PriceGenerationComparer() =
    interface IComparer<Order> with
        member this.Compare(left, right) =
            match left.Price.CompareTo(right.Price) with
            | 1 -> 1
            | -1 -> -1
            | _ -> left.Generation.CompareTo(right.Generation)

type Trade =
    {   Buyer:string
        Seller:string
        Instrument:string
        Quantity:Int64
        Price:decimal }

type OrderBook =
    {   BuyOrders : IntervalHeap<Order>
        SellOrders : IntervalHeap<Order>    }


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
         RemainingQuantity = 0
         Price = Decimal.Parse(fields.[3])
         Generation = generation }

let parseOrder text =
    text
    |> tokeniseOrderText
    |> Result.bind validateNumberOfFields
    |> Result.bind createOrder
        
let orderBooks = new Dictionary<string, OrderBook>()

let getOrderBook instrument =
    let exists, orderBook = orderBooks.TryGetValue(instrument)
    match exists with
    | true -> orderBook
    | false -> 
        let orderBook = {
            BuyOrders = new IntervalHeap<Order>(PriceGenerationComparer())
            SellOrders = new IntervalHeap<Order>(PriceGenerationComparer()) 
        }
        orderBooks.Add(instrument, orderBook)
        orderBook        

let insertOrder (order:Order) =
    let orderBook = getOrderBook order.Instrument
    let side =
        match order.Quantity with
        | quantity when quantity < 0L -> orderBook.SellOrders
        | _ -> orderBook.BuyOrders
    match side.Add(order) with
    | true -> ()
    | false -> failwith (sprintf "failed to add order %A" order)
    orderBook
    
let matchingOrders orderBook =
    if orderBook.BuyOrders.IsEmpty || orderBook.SellOrders.IsEmpty then
        None
    else
        let buyOrder = orderBook.BuyOrders.FindMax()
        let sellOrder = orderBook.SellOrders.FindMin()
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
    let buyOrder = orderBook.BuyOrders.DeleteMax()
    let sellOrder = orderBook.SellOrders.DeleteMin()
    let buyOrder = { buyOrder with Quantity = buyOrder.Quantity - trade.Quantity }
    let sellOrder = { sellOrder with Quantity = sellOrder.Quantity + trade.Quantity }
    if buyOrder.Quantity <> 0L then
        orderBook.BuyOrders.Add(buyOrder) |> ignore
    else if sellOrder.Quantity <> 0L then
        orderBook.SellOrders.Add(sellOrder) |> ignore
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
    let line = String.Format("{0}:{1}:{2}:{3}:{4}\n", trade.Buyer, trade.Seller, trade.Instrument, trade.Quantity, trade.Price)
    Console.Write(line)

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

