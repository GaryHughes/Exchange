namespace Exchange.Tests

open System
open System.Collections.Generic
open Microsoft.VisualStudio.TestTools.UnitTesting
open Microsoft.VisualStudio.TestPlatform.TestHost
open Exchange


[<TestClass>]
type TestClass () =

    let assertTrue result =
        match result with
        | Ok _ -> Assert.IsTrue(true)
        | Error message -> Assert.IsTrue(false, message)

    let assertFalse result =
        match result with
        | Ok _ -> Assert.IsTrue(false)
        | Error message -> Assert.IsTrue(true, message)

    [<TestInitialize>]
    member this.Initialize () =
        let book = getOrderBook "AUDUSD"
        book.BuyOrders.Clear()
        book.SellOrders.Clear()

    [<TestMethod>]
    member this.TestParseOrderSuccessful () =
        assertTrue(parseOrder "A:AUDUSD:100:1.47")
       
    [<TestMethod>]
    member this.TestParseOrderWithEmptyBuffer () =
        assertFalse(parseOrder "")
       
    [<TestMethod>]
    member this.TestParseOrderWithNotEnoughFields () =
        assertFalse(parseOrder "A:AUDUSD:100")

    [<TestMethod>]
    member this.TestBuyOrdering () =
        
        insertOrder { Participant = "A"; Instrument = "AUDUSD"; Quantity = 100L; RemainingQuantity = 0L; Price = 10m; Generation = 1 } |> ignore
        insertOrder { Participant = "A"; Instrument = "AUDUSD"; Quantity = 100L; RemainingQuantity = 0L; Price = 7m; Generation = 2 } |> ignore
        insertOrder { Participant = "A"; Instrument = "AUDUSD"; Quantity = 100L; RemainingQuantity = 0L; Price = 11m; Generation = 3 } |> ignore
        insertOrder { Participant = "A"; Instrument = "AUDUSD"; Quantity = 100L; RemainingQuantity = 0L; Price = 9m; Generation = 4 } |> ignore

        let book = getOrderBook "AUDUSD"

        Assert.AreEqual(11m, book.BuyOrders.[0].Price)
        Assert.AreEqual(10m, book.BuyOrders.[1].Price)
        Assert.AreEqual(9m, book.BuyOrders.[2].Price)
        Assert.AreEqual(7m, book.BuyOrders.[3].Price)

    [<TestMethod>]
    member this.TestBuyOrderingSamePrice () =
        insertOrder { Participant = "A"; Instrument = "AUDUSD"; Quantity = 100L; RemainingQuantity = 0L; Price = 10m; Generation = 1 } |> ignore
        insertOrder { Participant = "A"; Instrument = "AUDUSD"; Quantity = 101L; RemainingQuantity = 0L; Price = 10m; Generation = 2 } |> ignore
        insertOrder { Participant = "A"; Instrument = "AUDUSD"; Quantity = 102L; RemainingQuantity = 0L; Price = 10m; Generation = 3 } |> ignore
        insertOrder { Participant = "A"; Instrument = "AUDUSD"; Quantity = 103L; RemainingQuantity = 0L; Price = 10m; Generation = 4 } |> ignore
    
        let book = getOrderBook "AUDUSD"

        Assert.AreEqual(100L, book.BuyOrders.[0].Quantity)
        Assert.AreEqual(101L, book.BuyOrders.[1].Quantity)
        Assert.AreEqual(102L, book.BuyOrders.[2].Quantity)
        Assert.AreEqual(103L, book.BuyOrders.[3].Quantity)

    [<TestMethod>]
    member this.TestSellOrdering () =

        insertOrder { Participant = "A"; Instrument = "AUDUSD"; Quantity = -100L; RemainingQuantity = 0L; Price = 10m; Generation = 1 } |> ignore
        insertOrder { Participant = "A"; Instrument = "AUDUSD"; Quantity = -100L; RemainingQuantity = 0L; Price = 7m; Generation = 2 } |> ignore
        insertOrder { Participant = "A"; Instrument = "AUDUSD"; Quantity = -100L; RemainingQuantity = 0L; Price = 11m; Generation = 3 } |> ignore
        insertOrder { Participant = "A"; Instrument = "AUDUSD"; Quantity = -100L; RemainingQuantity = 0L; Price = 9m; Generation = 4 } |> ignore

        let book = getOrderBook "AUDUSD"

        Assert.AreEqual(7m, book.SellOrders.[0].Price)
        Assert.AreEqual(9m, book.SellOrders.[1].Price)
        Assert.AreEqual(10m, book.SellOrders.[2].Price)
        Assert.AreEqual(11m, book.SellOrders.[3].Price)

    [<TestMethod>]
    member this.TestSellOrderingSamePrice () =
        
        insertOrder { Participant = "A"; Instrument = "AUDUSD"; Quantity = -100L; RemainingQuantity = 0L; Price = 10m; Generation = 1 } |> ignore
        insertOrder { Participant = "A"; Instrument = "AUDUSD"; Quantity = -101L; RemainingQuantity = 0L; Price = 10m; Generation = 2 } |> ignore
        insertOrder { Participant = "A"; Instrument = "AUDUSD"; Quantity = -102L; RemainingQuantity = 0L; Price = 10m; Generation = 3 } |> ignore
        insertOrder { Participant = "A"; Instrument = "AUDUSD"; Quantity = -103L; RemainingQuantity = 0L; Price = 10m; Generation = 4 } |> ignore

        let book = getOrderBook "AUDUSD"

        Assert.AreEqual(-100L, book.SellOrders.[0].Quantity)
        Assert.AreEqual(-101L, book.SellOrders.[1].Quantity)
        Assert.AreEqual(-102L, book.SellOrders.[2].Quantity)
        Assert.AreEqual(-103L, book.SellOrders.[3].Quantity)