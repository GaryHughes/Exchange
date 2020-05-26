#! /bin/sh

set -e # Exit if there are any errors

. float_utils.sh
. file_utils.sh
. array_utils.sh

# Where we put the temorary data
dir=/tmp/book.$$
mkdir $dir
cd $dir
# Make sure the temporary directory is cleaned up
trap 'cd /tmp; rm -r $dir' 0

# Two sort commands, depending on buy vs sell.  Comparisons
# are numeric on price.  For buys, the price comparison is 
# reversed so the highest price is first.
# -s says stable sort (should be the default!)
ssort="sort -s  -n "
bsort="sort -s -nr "

maxprice=9999.9999

# Main loop
# awk command does 2 things:
# - converts from colon-separated to space-separated
# - drops the blank line at the end of some input files

awk -F: 'NF > 3 {print $1, $2, $3, $4}' | \
 while read user symbol qty price; do
    # remove the "/" from some symbols into something that
    # is both a valid filename and a valid shell variable name
    basename=${symbol/\//_}
    bfname=$basename.B 
    sfname=$basename.S

    set_array_key bestbid $basename 0
    set_array_key bestoffer $basename $maxprice

    if [ $qty -lt 0 ]; then
        side=S
        qty=$(( 0 - $qty ))
        sort=$ssort
        less_than $price ${!bestoffer} && set_array_value bestoffer $price
    else 
        side=B
        sort=$bsort
        less_than ${!bestbid} $price && set_array_value bestbid $price
    fi

    # Add to file
    echo $price $user $qty >> $basename.$side

    less_than ${!bestbid} ${!bestoffer} && continue
    
    # have overlap, so sort the file then process any trades

    $bsort $bfname > $bfname.new
    mv $bfname.new $bfname
    
    $ssort $sfname > $sfname.new
    mv $sfname.new $sfname

    read bprice buser bqty < $bfname
    read sprice suser sqty < $sfname 

    while [ -s $bfname -a -s $sfname ] ; do
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
            echo "$buser:$suser:$symbol:$tqty:$tprice"
            bqty=$(($bqty - $sqty))
            remove_first_line $sfname
            [ -s $sfname ] && read sprice suser sqty < $sfname
            replace_first_line $bfname $bprice $buser $bqty
        elif [ $sqty -gt $bqty ]; then
            tqty=$bqty
            echo "$buser:$suser:$symbol:$tqty:$tprice"
            sqty=$(($sqty - $bqty))
            remove_first_line $bfname
            [ -s $bfname ] && read bprice buser bqty < $bfname
            replace_first_line $sfname $sprice $suser $sqty
        else
            # same size
            tqty=$bqty
            echo "$buser:$suser:$symbol:$tqty:$tprice"
            remove_first_line $sfname
            remove_first_line $bfname
            read bprice buser bqty < $bfname
            read sprice suser sqty < $sfname
        fi


    done
    set_array_value bestbid $bprice
    set_array_value bestoffer $sprice
done
