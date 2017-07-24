# Exchange

# Part 3

Add the following functionality to your Exchange program.

* Change the format to include an order type field after the symbol 
    * The only supported type for now is limit 'L'
    * A limit order describes the existing behaviour of the Exchange 

        A:AUDUSD:­L:100:1.47

* Add the ability to handle market orders
    * Market orders are specified with a type 'M' 
    * A market order specifies a price of 0
    * 

        A:AUDUSD:­M:100:0

* Add the ability to specify a Minimum quantity for an order.

        A:AUDUSD:­100:1.47:50

* Immediate or Cancel

        A:AUDUSD:­100:1.47