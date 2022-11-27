#!/bin/bash

# shellcheck disable=SC2016

THREADS=(1 2 4 8 16 32 64)

echo "nb philosophs,nb threads,Mesure i,time" &> measure_philo.csv

NB_PHILOSOPHS=(3 10 101 500)
for thread in "${THREADS[@]}"; do
  for nb_phil in "${NB_PHILOSOPHS[@]}"; do  #in NB_PHILOSOPHS ?
    for i in {1..5}; do
      make philo -j "$thread" -s
      /usr/bin/time -f "$nb_phil,$thread,$i,%E" ./philo "$nb_phil" >>measure_philo.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
      make -s clean
    done
  done
done


echo "nb_cons,nb_prod,nb threads,Mesure i,time" &> measure_prodcons.csv

for thread in "${THREADS[@]}"; do
  for tot in {2..64} ; do
    two=2
    c=$((tot / two))
    p=$((tot / two))
    if [ $((tot%2)) != 0 ]; then    #comme que des thread pair, on pourrait le rerirer
      c=$((c+1))
    fi
    for i in {1..5}; do
      make prodcons -j "$thread" -s
      /usr/bin/time -f "$c,$p,$thread,$i,%E" ./prodcons "$c" "$p" >>measure_prodcons.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
      make -s clean
    done
  done
done

echo "nb_reader,nb_writer,nb threads,Mesure i,time" &> measure_rw.csv

for thread in "${THREADS[@]}"; do
  for tot in {2..64} ; do
    two=2
    r=$((tot / two))
    w=$((tot / two))
    if [ $((tot%2)) != 0 ]; then
      r=$((r+1))
    fi
    for i in {1..5}; do
      make rw -j "$thread" -s
      /usr/bin/time -f "$r,$w,$thread,$i,%E" ./rw "$r" "$w" >>measure_rw.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
      make -s clean
    done
  done
done

#Pour donner permission au script, executer $chmod +x script.sh
