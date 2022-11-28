

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

zip:
	tar -zcvf prog.tar.gz philosphe.c rw.c prodcons.c Makefile README.md

all:
	make philo
	make prodcons
	make rw

clean:
	rm -f *.o
	rm -f philo
	rm -f prodcons
	rm -f rw

.PHONY: clean all
