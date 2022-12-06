#!/bin/bash

# shellcheck disable=SC2016

NTHREADS=$(nproc)
MAX=$((NTHREADS*2))
THREADS=($(for ((i=1;i<=$MAX;i*=2)); do echo "${i}"; done))

echo "nb philosophs,nb threads,Mesure i,time" &> measures/measure_philottas.csv

for thread in "${THREADS[@]}"; do
    for i in {1..10}; do
      make philottas -j "$thread" -s
      /usr/bin/time -f "$thread,$thread,$i,%e" ./philottas "$thread" >>measures/measure_philottas.csv 2>&1
      make -s clean
    done
done


echo "nb_cons,nb_prod,nb threads,Mesure i,time" &> measures/measure_prodconsttas.csv

for thread in "${THREADS[@]}"; do
    if [ "$thread" != 1 ]; then
      c=$((thread/2))
      p=$((thread/2))
      for i in {1..10}; do
            make prodconsttas -j "$thread" -s
            /usr/bin/time -f "$c,$p,$thread,$i,%e" ./prodconsttas "$c" "$p" >>measures/measure_prodconsttas.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
            make -s clean
      done
    fi
done

echo "nb_reader,nb_writer,nb threads,Mesure i,time" &> measures/measure_rwttas.csv

for thread in "${THREADS[@]}"; do
    if [ "$thread" != 1 ]; then
      r=$((thread/2))
      w=$((thread/2))
      for i in {1..10}; do
            make rwttas -j "$thread" -s
            /usr/bin/time -f "$r,$w,$thread,$i,%e" ./rwttas "$r" "$w" >>measures/measure_rwttas.csv 2>&1
            make -s clean
      done
    fi
done