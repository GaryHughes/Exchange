#!/bin/bash

cp README-header.md README.md

if [ -f part_1_results.md ]; then
    echo "##Part 1 Performance" >> README.md
    cat part_1_results.md >> README.md
fi

if [ -f part_2_results.md ]; then
    echo "##Part 2 Performance" >> README.md
    cat part_2_results.md >> README.md
fi