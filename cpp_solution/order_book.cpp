#include "order_book.h"

namespace ae
{

void order_book::insert(const order& order)
{
	order.bump_generation();

	if(order.is_buy())
	{
		buy_order_collection::iterator buy;

		for (buy = m_buy_orders.begin(); buy != m_buy_orders.end(); ++buy)
		{
			if (buy->price() < order.price())
				break;
		}

		m_buy_orders.insert(buy, order);
	}
	else
	{
		sell_order_collection::iterator sell;		

		for (sell = m_sell_orders.begin(); sell != m_sell_orders.end(); ++sell)
		{
			if (sell->price() > order.price())
				break;
		}

		m_sell_orders.insert(sell, order);
	}
}

void order_book::match(trade_collection& trades)
{
	while(!m_buy_orders.empty() && !m_sell_orders.empty())
	{
		auto& buy_order = *m_buy_orders.begin();
		auto& sell_order = *m_sell_orders.begin();

		auto buy_price = buy_order.price();
		auto sell_price = sell_order.price();

		if(buy_price < sell_price)
			break;

		auto match_price = buy_order.generation() < sell_order.generation() ? buy_price : sell_price;

		auto match_quantity = std::min(buy_order.remaining_quantity(), sell_order.remaining_quantity());

		trades.emplace_back(ae::trade(buy_order.participant(),
								  	  sell_order.participant(),
									  buy_order.instrument(),
									  match_quantity,
									  match_price));

		if(buy_order.fill(match_quantity) == 0)
			m_buy_orders.erase(m_buy_orders.begin());

		if(sell_order.fill(match_quantity) == 0)
			m_sell_orders.erase(m_sell_orders.begin());
	}
}

} // namespace ae
