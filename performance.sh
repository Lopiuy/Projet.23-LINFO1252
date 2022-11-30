#!/bin/bash

# shellcheck disable=SC2016

THREADS=(1 2 4 8 16 32 64)

echo "nb philosophs,nb threads,Mesure i,time" &> measure_philo.csv

for thread in "${THREADS[@]}"; do
    for i in {1..5}; do
      make philo -j "$thread" -s
      /usr/bin/time -f "$thread,$thread,$i,%e" ./philo "$thread" >>measure_philo.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
      make -s clean
    done
done


echo "nb_cons,nb_prod,nb threads,Mesure i,time" &> measure_prodcons.csv

for thread in "${THREADS[@]}"; do
    if [ "$thread" != 1 ]; then    #comme que des thread pair, on pourrait le rerirer
      two=2
      c=$((thread / two))
      p=$((thread / two))
      for i in {1..5}; do
            make prodcons -j "$thread" -s
            /usr/bin/time -f "$c,$p,$thread,$i,%e" ./prodcons "$c" "$p" >>measure_prodcons.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
            make -s clean
      done
    fi
done

echo "nb_reader,nb_writer,nb threads,Mesure i,time" &> measure_rw.csv

for thread in "${THREADS[@]}"; do
    if [ "$thread" != 1 ]; then    #comme que des thread pair, on pourrait le rerirer
      two=2
      r=$((thread / two))
      w=$((thread / two))
      for i in {1..5}; do
            make rw -j "$thread" -s
            /usr/bin/time -f "$r,$w,$thread,$i,%e" ./rw "$r" "$w" >>measure_rw.csv 2>&1                    # s = silent (do not print commands in terminal), j = number of jobs(threads)
            make -s clean
      done
    fi
done

#Pour donner permission au script, executer $chmod +x script.sh
