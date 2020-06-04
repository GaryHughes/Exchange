namespace Exchange.Tests

open System
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

    [<TestMethod>]
    member this.TestParseOrderSuccessful () =
        assertTrue(parseOrder "A:AUDUSD:100:1.47")
       
    [<TestMethod>]
    member this.TestParseOrderWithEmptyBuffer () =
        assertFalse(parseOrder "")
       
    [<TestMethod>]
    member this.TestParseOrderWithNotEnoughFields () =
        assertFalse(parseOrder "A:AUDUSD:100")