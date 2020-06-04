#include <iostream>
#include <exception>
#include <algorithm>
#include <iterator>
#include "exchange.h"

int main(int, char**)
{
	try
	{
		ae::exchange exchange;

		std::transform(std::istream_iterator<ae::order>(std::cin),
					   std::istream_iterator<ae::order>(),
					   std::ostream_iterator<ae::trade_collection>(std::cout, ""),
					   [&](const ae::order& order)
					   {
							return exchange.execute(order);
					   });
	}
	catch(std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
