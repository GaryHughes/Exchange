#ifndef ae_order_book_h
#define ae_order_book_h

#include <memory>
#include <vector>
#include <map>
#include "order.h"
#include "trade_collection.h"

namespace ae
{

class order_book
{
private:

	using buy_order_collection = std::multimap<price_type, order, std::greater<price_type>>;
	using sell_order_collection = std::multimap<price_type, order>;

	buy_order_collection m_buy_orders;
	sell_order_collection m_sell_orders;

public:

	void insert(const order& order);
	void match(trade_collection& trades);

	const buy_order_collection& buy_orders() const { return m_buy_orders; }
	const sell_order_collection& sell_orders() const { return m_sell_orders; }

};

using order_book_ptr = std::shared_ptr<order_book>;

}

#endif
