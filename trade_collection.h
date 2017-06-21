#ifndef ae_trade_collection_h
#define ae_trade_collection_h

#include <vector>
#include <iostream>
#include "trade.h"

namespace ae
{

typedef std::vector<trade> trade_collection;

std::ostream& operator<<(std::ostream& os, const trade_collection& trades);

}

#endif
