#!/bin/bash

# shellcheck disable=SC2016

NTHREADS=$(nproc)
MAX=$((NTHREADS*2))
THREADS=($(for ((i=1;i<=$MAX;i*=2)); do echo "${i}"; done))

echo "min,max,nb threads" &> measures/measure_ttastest.csv

for thread in "${THREADS[@]}"; do
    for i in {1..10}; do
        make ttastest -j "$thread" -s
        ./ttastest "$thread" >>measures/measure_ttastest.csv 2>&1
        make -s clean
    done
done
