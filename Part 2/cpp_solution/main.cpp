#include <iostream>
#include <exception>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <array>
#include "exchange.h"

static const std::string separator = ":";

int main(int, char**)
{
	try
	{
		ae::exchange exchange;

		const size_t buffer_size = 256;
		char buffer[buffer_size];

		char participant[32];
		char instrument[32];
		ae::quantity_type quantity;
		ae::price_type price; 

		while (std::fgets(buffer, buffer_size, stdin) != NULL)
		{
			auto result = sscanf(buffer, "%[^:]:%[^:]:%li:%lf", participant, instrument, &quantity, &price);
	
			if (result != 4)
			{
				std::ostringstream msg;
				msg << "could not parse line '" << buffer << "' result = " << result; 
				throw std::runtime_error(msg.str());
			}

			for(const auto& trade : exchange.execute(instrument, ae::order(participant, quantity, price)))
			{
				printf("%s:%s:%s:%ld:%g\n", 
					   trade.buyer().c_str(), 
					   trade.seller().c_str(), 
					   trade.instrument().c_str(), 
					   trade.match_quantity(), 
					   trade.match_price());
			}
		}
	}
	catch(std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
