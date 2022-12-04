

#all:
#	make philo
#	make prodcons
#	make rw
#	make testandset
#	make testandtestandset
#	make backoff
#	make ttastest
#	make scripts
#	make plot

philo: src/philosophe.c
	gcc -o philo -Wall src/philosophe.c

prodcons: src/prodcons.c
	gcc -o prodcons -Wall src/prodcons.c

rw: src/lectecriv.c
	gcc -o rw -Wall src/lectecriv.c

plot: plot.py
	python3 plot.py measures/measure_philo.csv 
	python3 plot.py measures/measure_prodcons.csv
	python3 plot.py measures/measure_rw.csv
	python3 plot.py measures/measure_tas.csv measures/measure_ttas.csv

testandset: src/tas.c
	gcc -o tas -Wall src/tas.c

testandtestandset: src/ttas.c
	gcc -o ttas -Wall src/ttas.c

dummy: src/dummysem.c
	gcc -o dummy src/dummysem.c src/mysem.c src/queue.c
	./dummy
	rm -f dummy

philotas: src/philotas.c
	gcc -o philotas -Wall src/philotas.c

philottas: src/philottas.c
	gcc -o philottas -Wall src/philottas.c

prodconstas: src/prodconstas.c
	gcc -o prodconstas -Wall src/prodconstas.c

prodconsttas: src/prodconsttas.c
	gcc -o prodconsttas -Wall src/prodconsttas.c

rwtas: src/lectecrivtas.c
	gcc -o rwtas -Wall src/lectecrivtas.c

rwttas: src/lectecrivttas.c
	gcc -o rwttas -Wall src/lectecrivttas.c

backoff: src/backoff.c
	gcc -o backoff -Wall src/backoff.c

ttastest: src/ttastest.c
	gcc -o ttastest -Wall src/ttastest.c

zip:
	tar -zcvf prog.tar.gz philosphe.c rw.c prodcons.c Makefile README.md

scripts: experiments.sh
	./experiments.sh

clean:
	rm -f *.o
	rm -f philo
	rm -f prodcons
	rm -f rw
	rm -f tas
	rm -f ttas
	rm -f dummy
	rm -f philotas
	rm -f philottas
	rm -f prodconstas
	rm -f prodconsttas
	rm -f rwtas
	rm -f rwttas
	rm -f backoff
	rm -f ttastest

.PHONY: clean all
