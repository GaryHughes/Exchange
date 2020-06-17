#! /bin/sh

for k in 100 200 500 1000 2000 5000 10000  ; do
    printf "%5sk orders: " $k
    gzcat ../../orders-10M.txt.gz| sed ${k}000q | time ./runner > /dev/null
done
