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

            Assert.AreEqual(11m, book.BuyOrders.Dequeue().Price);
            Assert.AreEqual(10m, book.BuyOrders.Dequeue().Price);
            Assert.AreEqual(9m, book.BuyOrders.Dequeue().Price);
            Assert.AreEqual(7m, book.BuyOrders.Dequeue().Price);
        }

        [TestMethod]
        public void TestBuyOrderingSamePrice()
        {
            var book = new OrderBook();

            book.Execute(new Order("A:AUDUSD:100:10"));
            book.Execute(new Order("A:AUDUSD:101:10"));
            book.Execute(new Order("A:AUDUSD:102:10"));
            book.Execute(new Order("A:AUDUSD:103:10"));

            Assert.AreEqual(100m, book.BuyOrders.Dequeue().Quantity);
            Assert.AreEqual(101m, book.BuyOrders.Dequeue().Quantity);
            Assert.AreEqual(102m, book.BuyOrders.Dequeue().Quantity);
            Assert.AreEqual(103m, book.BuyOrders.Dequeue().Quantity);
        }

        [TestMethod]
        public void TestSellOrdering()
        {
            var book = new OrderBook();

            book.Execute(new Order("A:AUDUSD:-100:10"));
            book.Execute(new Order("A:AUDUSD:-100:7"));
            book.Execute(new Order("A:AUDUSD:-100:11"));
            book.Execute(new Order("A:AUDUSD:-100:9"));

            Assert.AreEqual(7m, book.SellOrders.Dequeue().Price);
            Assert.AreEqual(9m, book.SellOrders.Dequeue().Price);
            Assert.AreEqual(10m, book.SellOrders.Dequeue().Price);
            Assert.AreEqual(11m, book.SellOrders.Dequeue().Price);
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
