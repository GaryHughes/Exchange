using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Linq;

namespace Exchange.Tests
{
    [TestClass]
    public class OrderTests
    {
        [TestMethod]
        public void TestConstructor()
        {
            var order = new Order("A:AUDUSD:100:1.47");
            Assert.AreEqual("A", order.Participant.ToString());
            Assert.AreEqual("AUDUSD", order.Instrument.ToString());
            Assert.AreEqual(100, order.Quantity);
            Assert.AreEqual(100, order.RemainingQuantity);
            Assert.AreEqual(1.47m, order.Price);
        }
    }
}