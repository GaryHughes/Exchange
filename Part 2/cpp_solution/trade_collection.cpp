#include "trade_collection.h"

namespace ae
{

std::ostream& operator<<(std::ostream& os, const trade_collection& trades)
{
	for(const auto& trade : trades)
	{
		os << trade << '\n';
	}

	return os;
}

}
