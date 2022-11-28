

philo: philosophe.c
	gcc -o philo -Wall philosophe.c

prodcons: philosophe.c
	gcc -o prodcons -Wall prodcons.c

rw: philosophe.c
	gcc -o rw -Wall lectecriv.c

plot: plot.py
	python3 plot.py measure_philo.csv 
	python3 plot.py measure_prodcons.csv
	python3 plot.py measure_rw.csv

testandset: tas.c
	gcc -o tas -Wall tas.c

testandtestandset: ttas.c
	gcc -o ttas -Wall ttas.c

zip:
	tar -zcvf prog.tar.gz philosphe.c rw.c prodcons.c Makefile README.md

all:
	make philo
	make prodcons
	make rw
	make testset

clean:
	rm -f *.o
	rm -f philo
	rm -f prodcons
	rm -f rw
	rm -f tas
	rm -f ttas

.PHONY: clean all
