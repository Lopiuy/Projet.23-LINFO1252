#!/bin/bash

NTHREADS=$(nproc)
MAX=$((NTHREADS*2))
THREADS=($(for ((i=1;i<=$MAX;i*=2)); do echo "${i}"; done))

echo "nb threads,measure i,time" &> measures/measure_tas.csv

for thread in "${THREADS[@]}"; do
    for i in {1..10}; do
        make tas -j "$thread" -s
        /usr/bin/time -f "$thread,$i,%e" ./tas "$thread" >>measures/measure_tas.csv 2>&1
        make -s clean
    done
done

echo "nb threads,measure i,time" &> measures/measure_ttas.csv

for thread in "${THREADS[@]}"; do
    for i in {1..10}; do
        make ttas -j "$thread" -s
        /usr/bin/time -f "$thread,$i,%e" ./ttas "$thread" >>measures/measure_ttas.csv 2>&1
        make -s clean
    done
done