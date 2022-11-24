#!/bin/bash

THREADS=(1 2 4 6 8)

echo "nb philosophs,nb threads,Mesure i,time" &> measure_philo.csv

NB_PHILOSOPHS=(3 10 100 500)
for thread in "${THREADS[@]}"; do
  for nb_phil in "${NB_PHILOSOPHS[@]}"; do
    for i in {1..5}; do
      make philo -j $thread -s
      /usr/bin/time -f "$nb_phil,$thread,$i,%E" ./philo $nb_phil >>measure_philo.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
      make -s clean
    done
  done
done



#Pour donner permission au script, executer $chmod +x script.sh
