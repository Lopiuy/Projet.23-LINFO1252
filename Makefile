

all:
	./experiments.sh
	make plot

philo: src/BaseProblems/philosophe.c
	gcc -o philo -Wall src/BaseProblems/philosophe.c

prodcons: src/BaseProblems/prodcons.c
	gcc -o prodcons -Wall src/BaseProblems/prodcons.c

rw: src/BaseProblems/lectecriv.c
	gcc -o rw -Wall src/BaseProblems/lectecriv.c

plot: plot.py
	python3 plot.py measures/measure_philo.csv 
	python3 plot.py measures/measure_prodcons.csv
	python3 plot.py measures/measure_rw.csv
	python3 plot.py measures/measure_tas.csv measures/measure_ttas.csv

testandset: src/tas.c
	gcc -o tas -Wall src/tas.c src/MyMutex/mytasmutex.c

testandtestandset: src/ttas.c
	gcc -o ttas -Wall src/ttas.c src/MyMutex/myttasmutex.c

dummy: src/dummysem.c
	gcc -o dummy src/dummysem.c src/MySemaphores/mysemttas.c src/MySemaphores/queue.c src/MyMutex/myttasmutex.c
	./dummy
	rm -f dummy

philotas: src/TasProblems/philotas.c
	gcc -o philotas -Wall src/TasProblems/philotas.c src/MyMutex/mytasmutex.c

philottas: src/TtasProblems/philottas.c
	gcc -o philottas -Wall src/TtasProblems/philottas.c src/MyMutex/myttasmutex.c

prodconstas: src/TasProblems/prodconstas.c
	gcc -o prodconstas -Wall src/TasProblems/prodconstas.c src/MySemaphores/mysemttas.c src/MySemaphores/queue.c src/MyMutex/mytasmutex.c

prodconsttas: src/TtasProblems/prodconsttas.c
	gcc -o prodconsttas -Wall src/TtasProblems/prodconsttas.c src/MySemaphores/mysemttas.c src/MySemaphores/queue.c src/MyMutex/myttasmutex.c

rwtas: src/TasProblems/lectecrivtas.c
	gcc -o rwtas -Wall src/TasProblems/lectecrivtas.c src/MySemaphores/mysemttas.c src/MySemaphores/queue.c src/MyMutex/mytasmutex.c

rwttas: src/TtasProblems/lectecrivttas.c
	gcc -o rwttas -Wall src/TtasProblems/lectecrivttas.c src/MySemaphores/mysemttas.c src/MySemaphores/queue.c src/MyMutex/myttasmutex.c

backoff: src/backoff.c
	gcc -o backoff -Wall src/backoff.c src/MyMutex/mybackoffmutex.c

ttastest: src/ttastest.c
	gcc -o ttastest -Wall src/ttastest.c

zip:
	tar -zcvf Projet23-LINFO1252.tar.gz src scripts measures headers plot.py experiments.sh Makefile README.md

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
