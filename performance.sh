#!/bin/bash

echo "Nb Threads,Mesure i,time"

THREADS=(1 2 4 6 8)
#PROBLEMS=("philosophe" "prodcons" "rw")

#for prob in   "${PROBLEMS[@]}"; do
  for thread in "${THREADS[@]}"; do
    for i in {1..5}; do
      make philo -j $thread -s
      /usr/bin/time -f "philosophe,$thread,$i,%E" ./philo $i                      # s = silent (do not print commands in terminal), j = number of jobs(threads)
      make -s clean
    done
  done
#done

#Pour donner permission au script, executer $chmod +x script.sh

# sed "s/ar.*)//"

# ./script.sh &> meausure.csv