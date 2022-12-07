#!/bin/bash

NTHREADS=$(nproc)
MAX=$((NTHREADS*2))
THREADS=($(for ((i=1;i<=$MAX;i*=2)); do echo "${i}"; done))

echo "nb philosophs,nb threads,Mesure i,time" &> measures/measure_philo.csv

for thread in "${THREADS[@]}"; do
    for i in {1..10}; do
      make philo -j "$thread" -s
      /usr/bin/time -f "$thread,$thread,$i,%e" ./philo "$thread" >>measures/measure_philo.csv 2>&1
      make -s clean
    done
done


echo "nb_cons,nb_prod,nb threads,Mesure i,time" &> measures/measure_prodcons.csv

for thread in "${THREADS[@]}"; do
    if [ "$thread" != 1 ]; then
      c=$((thread/2))
      p=$((thread/2))
      for i in {1..10}; do
            make prodcons -j "$thread" -s
            /usr/bin/time -f "$c,$p,$thread,$i,%e" ./prodcons "$c" "$p" >>measures/measure_prodcons.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
            make -s clean
      done
    fi
done

echo "nb_reader,nb_writer,nb threads,Mesure i,time" &> measures/measure_rw.csv

for thread in "${THREADS[@]}"; do
    if [ "$thread" != 1 ]; then
      r=$((thread/2))
      w=$((thread/2))
      for i in {1..10}; do
            make rw -j "$thread" -s
            /usr/bin/time -f "$r,$w,$thread,$i,%e" ./rw "$r" "$w" >>measures/measure_rw.csv 2>&1
            make -s clean
      done
    fi
done