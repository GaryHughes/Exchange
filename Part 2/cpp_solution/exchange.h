#ifndef ae_exchange_h
#define ae_exchange_h

#include <unordered_map>
#include "order_book.h"

namespace ae
{

class exchange
{
public:

	trade_collection execute(const std::string& instrument, const ae::order& order);

private:

	order_book& order_book_for_instrument(const std::string& instrument);

	std::unordered_map<std::string, order_book> m_order_books;

};

}

#endif
