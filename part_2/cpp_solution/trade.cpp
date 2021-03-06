#include "trade.h"

namespace ae
{

trade::trade(const std::string& buyer,
		  	 const std::string& seller,
			 quantity_type match_quantity,
			 price_type match_price)
:	m_buyer(buyer),
	m_seller(seller),
	m_match_quantity(match_quantity),
	m_match_price(match_price)
{
}

} // namespace ae
