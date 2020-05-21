#! /bin/sh

set -e # Exit if there are any errors

. float_utils.sh
. file_utils.sh

# Where we put the temorary data
dir=/tmp/book.$$
mkdir $dir
cd $dir
# Make sure the temporary directory is cleaned up
trap 'cd /tmp; rm -r $dir' 0

# Two sort commands, depending on buy vs sell.  Comparisons
# are numeric on price.  For buys, the price comparison is 
# reversed so the highest price is first.
ssort="sort +0n -1"
bsort="sort +0nr -1"


# Main loop
# awk command does 2 things:
# - converts from colon-separated to space-separated
# - drops the blank line at the end of some input files

awk -F: 'NF > 3 {print $1, $2, $3, $4}' | \
 while read user symbol qty price; do
    if [ $qty -lt 0 ]; then
        side=S
        qty=$(( 0 - $qty ))
        sort=$ssort
    else 
        side=B
        sort=$bsort
    fi
    
    # remove the "/" from some symbols into something that
    # is both a valid filename and a valid shell variable name
    basename=${symbol/\//_}
    fname=$basename.$side
    bfname=$basename.B 
    sfname=$basename.S

    : Add to file and sort appropriately
    echo $price $user $qty >> $fname
    $sort $fname > $fname.new
    move $fname.new $fname

    
    while [ -s $bfname -a -s $sfname ] ; do
    
        read bprice buser bqty < $bfname
        read sprice suser sqty < $sfname 

        less_than $bprice $sprice && break

        # have overlap $bprice $sprice so print a trade.  
        # Price is the price of the first order (lower gen number)... 
        # which is the side opposite to the current record!
        case "$side" in 
        B) tprice=$sprice;;
        *) tprice=$bprice;;
        esac

        if [ $bqty -gt $sqty ]; then
            tqty=$sqty
            remove_first_line $sfname
            replace_first_line $bfname $bprice $buser $(($bqty - $sqty))
        elif [ $sqty -gt $bqty ]; then
            tqty=$bqty
            remove_first_line $bfname
            replace_first_line $sfname $sprice $suser $(($sqty - $bqty))
        else
            # same size
            tqty=$bqty
            remove_first_line $sfname
            remove_first_line $bfname
        fi

        echo "$buser:$suser:$symbol:$tqty:$tprice"

    done
done