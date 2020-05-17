#include "order.h"
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>

static const std::string separator = ":";

namespace ae
{

size_t order::s_next_generation = 0;

order::order(std::string&& part,
			 quantity_type qty,
			 price_type prc)
{
	participant(part);
	quantity(qty);
	price(prc);
}

void order::quantity(quantity_type value)
{
	if (value == 0)
	{
		throw std::runtime_error("0 is not valid quantity");
	}

	if (value > 0)
	{
		m_is_buy = true;
		m_quantity = value;
	}
	else
	{
		m_is_buy = false;
		m_quantity = -value;
	}

	m_remaining_quantity = m_quantity;
}

void order::price(price_type value)
{
	if (value == 0)
	{
		throw std::runtime_error("0 is not a valid price");
	}

	m_price = value;
}

order order::fill(quantity_type quantity) const
{
	if (quantity > m_remaining_quantity)
	{
		throw std::runtime_error("invalid attempt to fill more than the remaining quantity");
	}

	order result = *this;
	result.m_remaining_quantity -= quantity;
	return result;
}

} // namespace ae

