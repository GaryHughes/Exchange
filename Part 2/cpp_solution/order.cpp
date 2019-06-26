#include "order.h"
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>

static const std::string separator = ":";

namespace ae
{

size_t order::s_next_generation = 0;

order::order(const std::string& part,
		  	 const std::string& instr,
			 quantity_type qty,
			 price_type prc)
{
	participant(part);
	instrument(instr);
	quantity(qty);
	price(prc);
}

void order::quantity(quantity_type value)
{
	if(value == 0)
	{
		throw std::runtime_error("0 is not valid quantity");
	}

	if(value > 0)
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
	if(value == 0)
	{
		throw std::runtime_error("0 is not a valid price");
	}

	m_price = value;
}

quantity_type order::fill(quantity_type quantity)
{
	if(quantity > m_remaining_quantity)
	{
		throw std::runtime_error("invalid attempt to fill more than the remaining quantity");
	}

	m_remaining_quantity -= quantity;

	return m_remaining_quantity;
}

order order::parse(const char* buffer)
{
	char participant[32];
	char instrument[32];
	quantity_type quantity;
	price_type price; 

	auto result = sscanf(buffer, "%[^:]:%[^:]:%li:%lf", participant, instrument, &quantity, &price);
	
	if (result != 4)
	{
		std::ostringstream msg;
		msg << "could not parse line '" << buffer << "' result = " << result; 
		throw std::runtime_error(msg.str());
	}

	return order(participant, instrument, quantity, price);
}

std::ostream& operator<<(std::ostream& os, const order& o)
{
	os << o.participant() << ":"
	   << o.instrument() << ":" 
	   << o.quantity() << ":"
	   << o.price();
	return os;
}

} // namespace ae

