using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Linq;

namespace Exchange.Tests
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void TestBuyOrdering()
        {
            var book = new OrderBook();

            book.Execute(new Order("A:AUDUSD:100:10"));
            book.Execute(new Order("A:AUDUSD:100:7"));
            book.Execute(new Order("A:AUDUSD:100:11"));
            book.Execute(new Order("A:AUDUSD:100:9"));

            var buys = book.BuyOrders.Values.ToArray();

            Assert.AreEqual(11m, buys[0].Price);
            Assert.AreEqual(10m, buys[1].Price);
            Assert.AreEqual(9m, buys[2].Price);
            Assert.AreEqual(7m, buys[3].Price);
        }

        [TestMethod]
        public void TestBuyOrderingSamePrice()
        {
            var book = new OrderBook();

            book.Execute(new Order("A:AUDUSD:100:10"));
            book.Execute(new Order("A:AUDUSD:101:10"));
            book.Execute(new Order("A:AUDUSD:102:10"));
            book.Execute(new Order("A:AUDUSD:103:10"));

            var buys = book.BuyOrders.Values.ToArray();

            Assert.AreEqual(100m, buys[0].Quantity);
            Assert.AreEqual(101m, buys[1].Quantity);
            Assert.AreEqual(102m, buys[2].Quantity);
            Assert.AreEqual(103m, buys[3].Quantity);
        }

        [TestMethod]
        public void TestSellOrdering()
        {
            var book = new OrderBook();

            book.Execute(new Order("A:AUDUSD:-100:10"));
            book.Execute(new Order("A:AUDUSD:-100:7"));
            book.Execute(new Order("A:AUDUSD:-100:11"));
            book.Execute(new Order("A:AUDUSD:-100:9"));

            var sells = book.SellOrders.Values.ToArray();

            Assert.AreEqual(7m, sells[0].Price);
            Assert.AreEqual(9m, sells[1].Price);
            Assert.AreEqual(10m, sells[2].Price);
            Assert.AreEqual(11m, sells[3].Price);
        }

        [TestMethod]
        public void TestSellOrderingSamePrice()
        {
            var book = new OrderBook();

            book.Execute(new Order("A:AUDUSD:-100:10"));
            book.Execute(new Order("A:AUDUSD:-101:10"));
            book.Execute(new Order("A:AUDUSD:-102:10"));
            book.Execute(new Order("A:AUDUSD:-103:10"));

            var sells = book.SellOrders.Values.ToArray();

            Assert.AreEqual(-100, sells[0].Quantity);
            Assert.AreEqual(-101, sells[1].Quantity);
            Assert.AreEqual(-102, sells[2].Quantity);
            Assert.AreEqual(-103, sells[3].Quantity);
        }
    }
}
