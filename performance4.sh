#!/bin/bash

# shellcheck disable=SC2016

THREADS=(2 4 8 16 32 64)

echo "min,max,nb threads" &> measures/measure_ttastest.csv

for thread in "${THREADS[@]}"; do
    for i in {1..10}; do
        make ttastest -j "$thread" -s
        ./ttastest "$thread" >>measures/measure_ttastest.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
        make -s clean
    done
done
