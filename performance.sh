#!/bin/bash

# shellcheck disable=SC2016

THREADS=(1 2 4 8 16 32 64)

echo "nb philosophs,Mesure i,time" &> measure_philo.csv

#NB_PHILOSOPHS=(3 10 100 500)
#for thread in "${THREADS[@]}"; do
  for nb_phil in "${THREADS[@]}"; do  #in NB_PHILOSOPHS ?
    for i in {1..5}; do
      make philo -j "$nb_phil" -s
      /usr/bin/time -f "$nb_phil,$i,%E" ./philo "$nb_phil" >>measure_philo.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
      make -s clean
    done
  done
#done


echo "nb_cons,nb_prod,nb threads,Mesure i,time" &> measure_prodcons.csv

for thread in "${THREADS[@]}"; do
  two=2
  c=$((thread / two))
  p=$((thread / two))
  if [ $((thread%2)) != 0 ]; then
    c=$((c+1))
  fi
    for i in {1..5}; do
      make prodcons -j "$thread" -s
      /usr/bin/time -f "$c,$p,$thread,$i,%E" ./prodcons "$c" "$p" >>measure_prodcons.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
      make -s clean
    done
done

echo "nb_reader,nb_writer,nb threads,Mesure i,time" &> measure_rw.csv

for thread in "${THREADS[@]}"; do
  two=2
  r=$((thread / two))
  w=$((thread / two))
  if [ $((thread%2)) != 0 ]; then
    r=$((r+1))
  fi
    for i in {1..5}; do
      make rw -j "$thread" -s
      /usr/bin/time -f "$r,$w,$thread,$i,%E" ./rw "$r" "$w" >>measure_rw.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
      make -s clean
    done
done

#Pour donner permission au script, executer $chmod +x script.sh
