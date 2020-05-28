
CFLAGS=-O3 --std=c11 -Werror
# -fsanitize=address -fno-omit-frame-pointer -g
CPPFLAGS=
DEPS= 
OBJS=main.o order.o order_list.o order_book.o book_list.o
TESTS=test_order test_orderlist test_orderbook test_booklist test_sizes

default:	Exchange

all:	Exchange tests

clean:
	@rm -f *.o Exchange  $(TESTS)
	
tests:	$(TESTS)

check: tests
	@./test_order
	@./test_orderlist
	@./test_orderbook
	@./test_booklist

Exchange: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) 

test_order: test_order.o order.o unity.o
test_orderlist: test_orderlist.o order_list.o order.o unity.o
test_orderbook: test_orderbook.o order_book.o order_list.o order.o unity.o
test_booklist: test_booklist.o book_list.o order_book.o order_list.o order.o unity.o

unity.o: unittest/unity.c
	$(CC) $(CFLAGS) -DUNITY_INCLUDE_DOUBLE=1 -c -o $@ $^ 
