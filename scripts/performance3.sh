#!/bin/bash

# shellcheck disable=SC2016

THREADS=(1 2 4 8 16 32 64)

echo "nb threads,measure i,time" &> measures/measure_backoff.csv

for thread in "${THREADS[@]}"; do
    for i in {1..5}; do
        make backoff -j "$thread" -s
        /usr/bin/time -f "$thread,$i,%e" ./backoff "$thread" >>measures/measure_backoff.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
        make -s clean
    done
done
