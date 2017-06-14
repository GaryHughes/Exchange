# Exchange

Write a command line application in any language you like that acts as a basic exchange, or “order matcher”, taking limit order input from stdin, matching orders, and sending trade output to stdout. The application should adhere to the following requirements.

* A limit order is a directive to buy or sell a quantity of an instrument at or, better than, a specified price, called the limit price.
A limit order is specified by the format, with a negative quantity indicating a sell:

        <buyer/sellerid>:<instrument>:<signed­quantity>:<limit­price>
    
    For example, an order to sell 100 AUD (against USD) at a price of 1.47 (or more) would be.

        A:AUDUSD:­100:1.47

* A match occurs between two limit orders when.
    * both orders are for the same instrument 
    * One order is a buy (+ve signed quantity) and the other is a sell (-ve signed quantity) 
    * The “buy” limit price is equal to or higher than the “sell” limit price

* The result of a match is a trade at the match price and match quantity. The match price is the limit price of the first order input of the two orders. The match quantity is the minimum of the two unsigned order quantities. Both orders are then considered filled an amount equal to the match quantity. If the unsigned order quantities are not equal, one order will have a remaining unfilled quantity, which is then a candidate for further matching.

    A trade is specified by the format:

        <buyerid>:<sellerid>:<instrument>:<match­quantity>:<match­price>

    For example, a trade of 50 AUD (against USD) at 1.47 with “A” the buyer and “B”the seller would be,

        A:B:AUDUSD:50:1.47

* Orders should be processed and matched in order of receipt.
* When multiple orders are candidates for matching against a new order, matching should be against the most aggressively priced candidate (lowest price for sells, highest price for buys). If multiple candidates have the most aggressive price, matching should occur against the candidate first received. This should be repeated until either all match candidates or the new order are completely filled.
* A buyer is allowed to trade with itself (ie. Orders do not need different buyer/seller ids to match)

The usage of the application should be as per the following example:

    cat orders.txt | ./Exchange > trades.txt

Examples:

Input

    A:AUDUSD:100:1.47
    B:AUDUSD:-50:1.45

Output:

    A:B:AUDUSD:50:1.47

Input:

    A:GBPUSD:100:1.66
    B:EURUSD:-100:1.11
    F:EURUSD:-50:1.1
    C:GBPUSD:-10:1.5
    C:GBPUSD:-20:1.6
    C:GBPUSD:-20:1.7
    D:EURUSD:100:1.11

Output:

    A:C:GBPUSD:10:1.66
    A:C:GBPUSD:20:1.66
    D:F:EURUSD:50:1.1
    D:B:EURUSD:50:1.11