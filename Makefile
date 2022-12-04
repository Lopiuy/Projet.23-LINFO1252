run = gcc -o $@ -Wall $^

all: ./experiments.sh
	./experiments.sh
	make plot

philo: src/BaseProblems/philosophe.c
	$(run)

prodcons: src/BaseProblems/prodcons.c
	$(run)

rw: src/BaseProblems/lectecriv.c
	$(run)

plot: plot.py measures/measure_philo.csv measures/measure_prodcons.csv measures/measure_rw.csv measures/measure_tas.csv measures/measure_ttas.csv
	python3 plot.py measures/measure_philo.csv 
	python3 plot.py measures/measure_prodcons.csv
	python3 plot.py measures/measure_rw.csv
	python3 plot.py measures/measure_tas.csv measures/measure_ttas.csv

tas: src/tas.c src/MyMutex/mytasmutex.c
	$(run)

ttas: src/ttas.c src/MyMutex/myttasmutex.c
	$(run)

dummy: src/dummysem.c src/MySemaphores/mysemttas.c src/MySemaphores/queue.c src/MyMutex/myttasmutex.c
	$(run)
	./dummy
	rm -f $@

philotas: src/TasProblems/philotas.c src/MyMutex/mytasmutex.c
	$(run)

philottas: src/TtasProblems/philottas.c src/MyMutex/myttasmutex.c
	$(run)

prodconstas: src/TasProblems/prodconstas.c src/MySemaphores/mysemttas.c src/MySemaphores/queue.c src/MyMutex/mytasmutex.c
	$(run)

prodconsttas: src/TtasProblems/prodconsttas.c src/MySemaphores/mysemttas.c src/MySemaphores/queue.c src/MyMutex/myttasmutex.c
	$(run)

rwtas: src/TasProblems/lectecrivtas.c src/MySemaphores/mysemttas.c src/MySemaphores/queue.c src/MyMutex/mytasmutex.c
	$(run)

rwttas: src/TtasProblems/lectecrivttas.c src/MySemaphores/mysemttas.c src/MySemaphores/queue.c src/MyMutex/myttasmutex.c
	$(run)

backoff: src/backoff.c src/MyMutex/mybackoffmutex.c
	$(run)

ttastest: src/ttastest.c
	$(run)

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
