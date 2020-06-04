using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Linq;

namespace Exchange.Tests
{
    [TestClass]
    public class OrderBookTests
    {
        [TestMethod]
        public void TestBuyOrdering()
        {
            var book = new OrderBook();

            book.Execute(new Order("A:AUDUSD:100:10"));
            book.Execute(new Order("A:AUDUSD:100:7"));
            book.Execute(new Order("A:AUDUSD:100:11"));
            book.Execute(new Order("A:AUDUSD:100:9"));

            Assert.AreEqual(11, book.BuyOrders.Dequeue().Price);
            Assert.AreEqual(10, book.BuyOrders.Dequeue().Price);
            Assert.AreEqual(9, book.BuyOrders.Dequeue().Price);
            Assert.AreEqual(7, book.BuyOrders.Dequeue().Price);
        }

        [TestMethod]
        public void TestBuyOrderingSamePrice()
        {
            var book = new OrderBook();

            book.Execute(new Order("A:AUDUSD:100:10"));
            book.Execute(new Order("A:AUDUSD:101:10"));
            book.Execute(new Order("A:AUDUSD:102:10"));
            book.Execute(new Order("A:AUDUSD:103:10"));

            Assert.AreEqual(100, book.BuyOrders.Dequeue().Quantity);
            Assert.AreEqual(101, book.BuyOrders.Dequeue().Quantity);
            Assert.AreEqual(102, book.BuyOrders.Dequeue().Quantity);
            Assert.AreEqual(103, book.BuyOrders.Dequeue().Quantity);
        }

        [TestMethod]
        public void TestSellOrdering()
        {
            var book = new OrderBook();

            book.Execute(new Order("A:AUDUSD:-100:10"));
            book.Execute(new Order("A:AUDUSD:-100:7"));
            book.Execute(new Order("A:AUDUSD:-100:11"));
            book.Execute(new Order("A:AUDUSD:-100:9"));

            Assert.AreEqual(7, book.SellOrders.Dequeue().Price);
            Assert.AreEqual(9, book.SellOrders.Dequeue().Price);
            Assert.AreEqual(10, book.SellOrders.Dequeue().Price);
            Assert.AreEqual(11, book.SellOrders.Dequeue().Price);
        }

        [TestMethod]
        public void TestSellOrderingSamePrice()
        {
            var book = new OrderBook();

            book.Execute(new Order("A:AUDUSD:-100:10"));
            book.Execute(new Order("A:AUDUSD:-101:10"));
            book.Execute(new Order("A:AUDUSD:-102:10"));
            book.Execute(new Order("A:AUDUSD:-103:10"));

            Assert.AreEqual(-100, book.SellOrders.Dequeue().Quantity);
            Assert.AreEqual(-101, book.SellOrders.Dequeue().Quantity);
            Assert.AreEqual(-102, book.SellOrders.Dequeue().Quantity);
            Assert.AreEqual(-103, book.SellOrders.Dequeue().Quantity);
        }
    }
}
