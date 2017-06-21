#include "order.h"
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

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

std::istream& operator>>(std::istream& is, order& o)
{
	std::string line;

	for(;;)
	{
		if(!std::getline(is, line))
			return is;

		if(!line.empty())
			break;
	}

	enum
	{
		token_participant,
		token_instrument,
		token_quantity,
		token_price,
		token_required
	};

	std::vector<std::string> tokens;

	boost::split(tokens, line, boost::is_any_of(separator));

	if(tokens.size() != token_required)
	{
		std::ostringstream msg;
		msg << "order::parse() input does not have " << token_required << " components";
		throw std::runtime_error(msg.str());
	}

	const std::string& participant = tokens[token_participant];
	const std::string& instrument = tokens[token_instrument];
	quantity_type quantity;
	price_type price;

	try
	{
		quantity = boost::lexical_cast<quantity_type>(tokens[token_quantity]);
	}
	catch(boost::bad_lexical_cast& ex)
	{
		std::ostringstream msg;
		msg << "quantity field '" << tokens[token_quantity] << "' is invalid";
		throw std::runtime_error(msg.str());
	}

	try
	{
		price = boost::lexical_cast<price_type>(tokens[token_price]);
	}
	catch(boost::bad_lexical_cast& ex)
	{
		std::ostringstream msg;
		msg << "price field '" << tokens[token_price] << "' is invalid";
		throw std::runtime_error(msg.str());
	}

	o.participant(participant);
	o.instrument(instrument);
	o.quantity(quantity);
	o.price(price);

	return is;
}

} // namespace ae

