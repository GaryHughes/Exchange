#ifndef ae_trade_collection_h
#define ae_trade_collection_h

#include <vector>
#include <iostream>
#include "trade.h"

namespace ae
{

using trade_collection = std::vector<trade>;

std::ostream& operator<<(std::ostream& os, const trade_collection& trades);

}

#endif
