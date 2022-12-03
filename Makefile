

philo: philosophe.c
	gcc -o philo -Wall philosophe.c

prodcons: prodcons.c
	gcc -o prodcons -Wall prodcons.c

rw: lectecriv.c
	gcc -o rw -Wall lectecriv.c

plot: plot.py
	python3 plot.py measure_philo.csv 
	python3 plot.py measure_prodcons.csv
	python3 plot.py measure_rw.csv
	python3 plot.py measure_tas.csv measure_ttas.csv

testandset: tas.c
	gcc -o tas -Wall tas.c

testandtestandset: ttas.c
	gcc -o ttas -Wall ttas.c

dummy: dummysem.c
	gcc -o dummy dummysem.c mysem.c queue.c

backoff: backoff.c
	gcc -o backoff -Wall backoff.c

ttastest: ttastest.c
	gcc -o ttastest -Wall ttastest.c

zip:
	tar -zcvf prog.tar.gz philosphe.c rw.c prodcons.c Makefile README.md

all:
	make philo
	make prodcons
	make rw
	make testandset
	make testandtestandset
	make backoff
	make ttastest

clean:
	rm -f *.o
	rm -f philo
	rm -f prodcons
	rm -f rw
	rm -f tas
	rm -f ttas
	rm -f dummy
	rm -f backoff
	rm -f ttastest

.PHONY: clean all
