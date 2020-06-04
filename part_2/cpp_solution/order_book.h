#ifndef ae_order_book_h
#define ae_order_book_h

#include <memory>
#include <vector>
#include <map>
#include <queue>
#include <limits>
#include <cmath>
#include "order.h"
#include "trade_collection.h"

namespace ae
{

inline bool prices_equal(const price_type& one, const price_type& two)
{
	return std::fabs(one - two) < std::numeric_limits<price_type>::epsilon();
}

inline bool buy_ordering(const order& lhs, const order& rhs)
{
	if (prices_equal(lhs.price(), rhs.price()))
		return lhs.generation() > rhs.generation();
	return lhs.price() < rhs.price();
}

inline bool sell_ordering(const order& lhs, const order& rhs)
{
	if (prices_equal(lhs.price(), rhs.price()))
		return lhs.generation() > rhs.generation();
	return lhs.price() > rhs.price();
}

class order_book
{
private:

	std::string m_instrument;

	using buy_order_collection = std::priority_queue<order, std::vector<order>, decltype(&buy_ordering)>;
	using sell_order_collection = std::priority_queue<order, std::vector<order>, decltype(&sell_ordering)>;

	buy_order_collection m_buy_orders;
	sell_order_collection m_sell_orders;

public:

	order_book();

	void insert(const order& order);
	trade_collection match();

	const buy_order_collection& buy_orders() const { return m_buy_orders; }
	const sell_order_collection& sell_orders() const { return m_sell_orders; }

	buy_order_collection& buy_orders() { return m_buy_orders; }
	sell_order_collection& sell_orders() { return m_sell_orders; }

};

}

#endif
