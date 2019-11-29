#!/bin/bash

cp README-header.md README.md

if [ -f part_1_results.md ]; then
    cat README_part_1_header.md >> README.md
    cat part_1_results.md >> README.md
fi

if [ -f part_2_results.md ]; then
    cat README_part_2_header.md >> README.md
    cat part_2_results.md >> README.md
fi