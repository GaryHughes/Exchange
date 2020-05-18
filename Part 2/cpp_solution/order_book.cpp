#include "order_book.h"

namespace ae
{

order_book::order_book(const std::string& instrument)
:	m_instrument(instrument)
,	m_buy_orders(buy_ordering)
,	m_sell_orders(sell_ordering)
{
}

const std::string& order_book::instrument() const
{
	return m_instrument;
}

void order_book::insert(const order& order)
{
	order.bump_generation();

	if(order.is_buy())
	{
		m_buy_orders.push(order);
	}
	else
	{
		m_sell_orders.push(order);
	}
}

trade_collection order_book::match()
{
	trade_collection trades;

	while(!m_buy_orders.empty() && !m_sell_orders.empty())
	{
		const auto& buy_order = m_buy_orders.top();
		const auto& sell_order = m_sell_orders.top();

		auto buy_price = buy_order.price();
		auto sell_price = sell_order.price();

		if(buy_price < sell_price)
			break;

		auto match_price = buy_order.generation() < sell_order.generation() ? buy_price : sell_price;

		auto match_quantity = std::min(buy_order.remaining_quantity(), sell_order.remaining_quantity());

		trades.emplace_back(buy_order.participant(),
							sell_order.participant(),
							match_quantity,
							match_price);
		
		auto buy_remainder = buy_order.fill(match_quantity);
		auto sell_remainder = sell_order.fill(match_quantity);

		m_buy_orders.pop();
		m_sell_orders.pop();

		if (buy_remainder.remaining_quantity() > 0)
			m_buy_orders.push(buy_remainder);

		if (sell_remainder.remaining_quantity() > 0)
			m_sell_orders.push(sell_remainder);
	}

	return trades;
}

} // namespace ae
