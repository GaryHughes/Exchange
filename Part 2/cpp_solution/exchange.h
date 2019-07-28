#ifndef ae_exchange_h
#define ae_exchange_h

#include <unordered_map>
#include "order_book.h"

namespace ae
{

class exchange
{
public:

	trade_collection execute(const order& order);

private:

	order_book_ptr order_book_for_instrument(const std::string& instrument);

	std::unordered_map<std::string, order_book_ptr> m_order_books;

};

}

#endif
