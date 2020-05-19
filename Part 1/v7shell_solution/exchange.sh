:

set -e 

dir=book.$$
mkdir $dir
trap 'rm -r $dir' 0

ssort="sort -n +0 -1 +1 -2"
bsort="sort -n +0r -1 +1 -2"

: Main loop

: " awk command does 2 things:
 - converts from colon-separated to space-separated
 - drops the blank line at the end of some input files
 The sed command converts / in some symbols to ~, 
 another sed command fixes it on the output
"
awk -F: 'NF > 3 {print NR, $1, $2, $3, $4}' | sed -e 's/\//~/g' | \
 while read gen user symbol qty price; do
    if test $qty -lt 0; then
        side=S
        qty=`expr $qty : '-\(.*\)'`
        sort=$ssort
    else 
        side=B
        sort=$bsort
    fi
    
    basename=$dir/$symbol
    fname=$basename.$side
    bfname=$basename.B 
    sfname=$basename.S

    : Add to file and sort appropriately
    echo $price $gen $user $qty >> $fname
    $sort $fname > $fname.new
    mv $fname.new $fname

    
    while test -s $bfname -a -s $sfname ; do
    
        read bprice bgen buser bqty < $bfname
        read sprice sgen suser sqty < $sfname 

        : "test cannot handle floating point for prices so need to be clever"
        cmp=`awk 'BEGIN { if ('$bprice' < '$sprice') print "less" ; else print "ge"; exit}'`
        
        case "$cmp" in
        less) break;;
        esac

        : "have overlap $bprice $sprice so print a trade.  
        Price is the price of the first order (lower gen number)... 
        which is the side opposite to the current record! "
        case "$side" in 
        B) tprice=$sprice;;
        *) tprice=$bprice;;
        esac

        if test $bqty -gt $sqty; then
            tqty=$sqty
            sed 1d < $sfname > $sfname.new 
            mv $sfname.new $sfname
            echo $bprice $bgen $buser `expr $bqty - $sqty` >> $bfname
            sed 1d $bfname | $bsort > $bfname.new
            if test -s $bfname.new; then
                mv $bfname.new $bfname
            else
                rm $bfname.new $bfname
            fi
        elif test $sqty -gt $bqty; then
            tqty=$bqty
            sed 1d < $bfname > $bfname.new 
            mv $bfname.new $bfname
            echo $sprice $sgen $suser `expr $sqty - $bqty` >> $sfname
            sed 1d $sfname | $ssort > $sfname.new
            if test -s $sfname.new; then
                mv $sfname.new $sfname
            else
                rm $sfname.new $sfname
            fi
        else
            : same size
            tqty=$bqty
            sed 1d < $sfname > $sfname.new 
            mv $sfname.new $sfname
            sed 1d < $bfname > $bfname.new 
            mv $bfname.new $bfname
        fi

        echo "$buser:$suser:$symbol:$tqty:$tprice"

    done
done | sed 's/~/\//'