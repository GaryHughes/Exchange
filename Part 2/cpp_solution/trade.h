#ifndef ae_trade_h
#define ae_trade_h

#include <iostream>
#include "types.h"

namespace ae
{

class trade
{
public:

	trade(const std::string& buyer,
		  const std::string& seller,
		  quantity_type match_quantity,
		  price_type match_price);

	const std::string& buyer() const { return m_buyer; }
	const std::string& seller() const { return m_seller; }
	quantity_type match_quantity() const { return m_match_quantity; }
	price_type match_price() const { return m_match_price; }

private:

	std::string m_buyer;
	std::string m_seller;
	quantity_type m_match_quantity;
	price_type m_match_price;

};

}

#endif
