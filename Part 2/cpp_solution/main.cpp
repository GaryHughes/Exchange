#include <iostream>
#include <exception>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <array>
#include <charconv>
#include <cstring>
#include "exchange.h"

static const std::string separator = ":";

int main(int, char**)
{
	try
	{
		ae::exchange exchange;

		const size_t buffer_size = 256;
		char buffer[buffer_size];

		ae::quantity_type quantity;
		ae::price_type price; 

		while (std::fgets(buffer, buffer_size, stdin) != NULL)
		{
			auto begin = buffer;

			auto end = std::strpbrk(begin, ":");
			if (!end) {
				break;
			}

			std::string_view participant(begin, end - begin);

			begin = ++end;

			end = std::strpbrk(begin, ":");
			if (!end) {
				throw std::runtime_error("no second token");
			}

			std::string_view instrument(begin, end - begin);

			begin = ++end;
			end = std::strpbrk(begin, ":");
			if (!end) {
				throw std::runtime_error("no third token");
			}

	        auto [ptr, ec] = std::from_chars(begin, end, quantity);
        
			if (ec != std::errc()) {
				throw std::out_of_range(std::string(begin, end) + " is not a valid quantity");
			}

			begin = ++end;
			end = std::strpbrk(begin, ":\n\r");
			if (!end) {
				throw std::runtime_error("no fourth token");
			}
			*end = 0;

			price = std::stod(begin);

			for (const auto& trade : exchange.execute(instrument, ae::order(participant, quantity, price)))
			{
				*(const_cast<char*>(instrument.begin()) + instrument.size()) = 0;
				printf("%s:%s:%s:%ld:%g\n", 
					   trade.buyer().c_str(), 
					   trade.seller().c_str(), 
					   instrument.begin(), 
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
