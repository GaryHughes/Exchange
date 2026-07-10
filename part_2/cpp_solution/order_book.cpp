#include "order_book.h"

namespace ae
{

order_book::order_book()
{
}

void order_book::insert(order&& order)
{
	order.bump_generation();

	if(order.is_buy())
	{
		m_buy_orders.push(std::move(order));
	}
	else
	{
		m_sell_orders.push(std::move(order));
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
		
		buy_order.reduce_remaining_quantity(match_quantity);
		sell_order.reduce_remaining_quantity(match_quantity);

		if (buy_order.remaining_quantity() == 0)
			m_buy_orders.pop();

		if (sell_order.remaining_quantity() == 0)
			m_sell_orders.pop();
	}

	return trades;
}

} // namespace ae
