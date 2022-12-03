#!/bin/bash

# shellcheck disable=SC2016

THREADS=(1 2 4 8 16 32 64)

echo "nb philosophs,nb threads,Mesure i,time" &> measures/measure_philotas.csv

for thread in "${THREADS[@]}"; do
    for i in {1..5}; do
      make philotas -j "$thread" -s
      /usr/bin/time -f "$thread,$thread,$i,%e" ./philotas "$thread" >>measures/measure_philotas.csv 2>&1
      make -s clean
    done
done


echo "nb_cons,nb_prod,nb threads,Mesure i,time" &> measures/measure_prodconstas.csv

for thread in "${THREADS[@]}"; do
    if [ "$thread" != 1 ]; then
      two=2
      c=$((thread / two))
      p=$((thread / two))
      for i in {1..5}; do
            make prodconstas -j "$thread" -s
            /usr/bin/time -f "$c,$p,$thread,$i,%e" ./prodconstas "$c" "$p" >>measures/measure_prodconstas.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
            make -s clean
      done
    fi
done

echo "nb_reader,nb_writer,nb threads,Mesure i,time" &> measures/measure_rwtas.csv

for thread in "${THREADS[@]}"; do
    if [ "$thread" != 1 ]; then
      two=2
      r=$((thread / two))
      w=$((thread / two))
      for i in {1..5}; do
            make rwtas -j "$thread" -s
            /usr/bin/time -f "$r,$w,$thread,$i,%e" ./rwtas "$r" "$w" >>measures/measure_rwtas.csv 2>&1
            make -s clean
      done
    fi
done