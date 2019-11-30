
CXX=clang++-7
CFLAGS=-I. -I/usr/local/include -std=c++14 -O3
DEPS = 
OBJS = order.o order_book.o trade.o trade_collection.o exchange.o
LIBS = -lboost_unit_test_framework

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

default:	Exchange

all:	Exchange tests

clean:
	@rm -f *.o Exchange order_tests exchange_tests
	
tests:	order_tests exchange_tests order_book_tests

check: tests
	@./order_tests
	@./exchange_tests
	@./order_book_tests
	

Exchange: main.o $(OBJS)
	$(CXX) -o $@ $^ $(CFLAGS)

exchange_tests:	exchange_tests.o $(OBJS)
	$(CXX) -o $@ $^ $(CFLAGS) $(LIBS)
	
order_tests:	order_tests.o $(OBJS)
	$(CXX) -o $@ $^ $(CFLAGS) $(LIBS)
	
order_book_tests:	order_book_tests.o $(OBJS)
	$(CXX) -o $@ $^ $(CFLAGS) $(LIBS)
