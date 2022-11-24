#!/bin/bash

# shellcheck disable=SC2016

echo "nb philosophs,nb threads,Mesure i,time" &> measure_philo.csv

NB_PHILOSOPHS=(3 10 100 500)
for thread in {1..8}; do
  for nb_phil in "${NB_PHILOSOPHS[@]}"; do
    for i in {1..5}; do
      make philo -j "$thread" -s
      /usr/bin/time -f "$nb_phil,$thread,$i,%E" ./philo "$nb_phil" >>measure_philo.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
      make -s clean
    done
  done
done


echo "nb_cons,nb_prod,nb threads,Mesure i,time" &> measure_prodcons.csv

for thread in {2..8}; do
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

#Pour donner permission au script, executer $chmod +x script.sh
