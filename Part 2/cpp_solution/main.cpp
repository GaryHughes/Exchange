#include <iostream>
#include <exception>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <array>
#include "exchange.h"


int main(int, char**)
{
	try
	{
		ae::exchange exchange;

		const size_t buffer_size = 256;
		char buffer[buffer_size];

		while (std::fgets(buffer, buffer_size, stdin) != NULL)
		{
			for (auto trade : exchange.execute(ae::order::parse(buffer)))
			{
				std::cout << trade << '\n';	
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
