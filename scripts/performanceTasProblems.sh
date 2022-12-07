#!/bin/bash

NTHREADS=$(nproc)
MAX=$((NTHREADS*2))
THREADS=($(for ((i=1;i<=$MAX;i*=2)); do echo "${i}"; done))

echo "nb philosophs,nb threads,Mesure i,time" &> measures/measure_philotas.csv

for thread in "${THREADS[@]}"; do
    for i in {1..10}; do
      make philotas -j "$thread" -s
      /usr/bin/time -f "$thread,$thread,$i,%e" ./philotas "$thread" >>measures/measure_philotas.csv 2>&1
      make -s clean
    done
done


echo "nb_cons,nb_prod,nb threads,Mesure i,time" &> measures/measure_prodconstas.csv

for thread in "${THREADS[@]}"; do
    if [ "$thread" != 1 ]; then
      c=$((thread/2))
      p=$((thread/2))
      for i in {1..10}; do
            make prodconstas -j "$thread" -s
            /usr/bin/time -f "$c,$p,$thread,$i,%e" ./prodconstas "$c" "$p" >>measures/measure_prodconstas.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
            make -s clean
      done
    fi
done

echo "nb_reader,nb_writer,nb threads,Mesure i,time" &> measures/measure_rwtas.csv

for thread in "${THREADS[@]}"; do
    if [ "$thread" != 1 ]; then
      r=$((thread/2))
      w=$((thread/2))
      for i in {1..10}; do
            make rwtas -j "$thread" -s
            /usr/bin/time -f "$r,$w,$thread,$i,%e" ./rwtas "$r" "$w" >>measures/measure_rwtas.csv 2>&1
            make -s clean
      done
    fi
done