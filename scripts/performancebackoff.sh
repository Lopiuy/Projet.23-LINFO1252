#!/bin/bash

# shellcheck disable=SC2016

NTHREADS=$(nproc)
MAX=$((NTHREADS*2))
THREADS=($(for ((i=1;i<=$MAX;i*=2)); do echo "${i}"; done))

echo "nb threads,measure i,time" &> measures/measure_backoff.csv

for thread in "${THREADS[@]}"; do
    for i in {1..5}; do
        make backoff -j "$thread" -s
        /usr/bin/time -f "$thread,$i,%e" ./backoff "$thread" >>measures/measure_backoff.csv 2>&1
        make -s clean
    done
done
