
CFLAGS=-O3 --std=c11 -Werror
# For the clang leak checker
# CFLAGS += -fsanitize=address -fno-omit-frame-pointer -g
# For gprof profiling
# CFLAGS += -pg -g
CPPFLAGS=
OBJS=main.o order_list.o order_book.o book_list.o
TESTS=test_orderlist test_orderbook test_booklist test_sizes

# For .h dependencies, implies clang/gcc
CFLAGS += -MMD

default:	Exchange

all:	Exchange tests

clean:
	@rm -f *.d *.o Exchange $(TESTS)
		
tests: $(TESTS)

check: tests
	@./test_orderlist
	@./test_orderbook
	@./test_booklist

Exchange: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) 

test_orderlist: test_orderlist.o order_list.o unity.o order_book.o
test_orderbook: test_orderbook.o order_book.o order_list.o unity.o
test_booklist: test_booklist.o book_list.o order_book.o order_list.o unity.o
test_sizes: test_sizes.o

unity.o: unittest/unity.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -DUNITY_INCLUDE_DOUBLE=1 -c -o $@ $<

# For dependency tracking
# gnu make version
include $(wildcard *.d)

# BSD make version
#.for f in $(OBJS:.o=.d) 
#.-include "$f"
#.endfor

# BSD Make also doesn't have this implict rule for linking a binary from multiple .o files
# Above test_xxx rely on this
#.o:
#        $(CC) $(CFLAGS) $(LDFLAGS) -o $@ ${.ALLSRC} $(LDLIBS) 
