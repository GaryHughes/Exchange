#include "exchange.h"

namespace ae
{

trade_collection exchange::execute(std::string_view instrument, const ae::order& order)
{
	ae::trade_collection trades;

	auto& book = order_book_for_instrument(std::string(instrument));

	book.insert(order);
	book.match(trades);

	return trades;
}

order_book& exchange::order_book_for_instrument(const std::string& instrument)
{
	auto existing = m_order_books.find(instrument);

	if(existing != m_order_books.end())
		return existing->second;

	return m_order_books.emplace(instrument, order_book(instrument)).first->second;
}

}
