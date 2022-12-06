#!/bin/bash

# shellcheck disable=SC2016

NTHREADS=$(nproc)
MAX=$((NTHREADS*2))
THREADS=($(for ((i=1;i<=$MAX;i*=2)); do echo "${i}"; done))

IT=$(python3 src/BackOff/backoffValue.py)

echo "nb threads,measure i,time" &> measures/measure_backoff.csv

for thread in "${THREADS[@]}"; do
  min=$((thread/2))
  for i in {1..10}; do
      make backoff -j "$thread" -s
      /usr/bin/time -f "$thread,$i,%e" ./backoff "$thread" "$min" "$IT" >>measures/measure_backoff.csv 2>&1
      make -s clean
  done
done
