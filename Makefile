run = gcc -o $@ -Wall $^  -lpthread


all:
	make philo
	make prodcons
	make rw
	make tas
	make ttas
	make philotas
	make philottas
	make prodconstas
	make prodconsttas
	make rwtas
	make rwttas
	make backoff
	make clean

philo: src/BaseProblems/philosophe.c
	$(run)

prodcons: src/BaseProblems/prodcons.c
	$(run)

rw: src/BaseProblems/lectecriv.c
	$(run)

plot: plot.py measures
	python3 plot.py measures/measure_philo.csv 
	python3 plot.py measures/measure_prodcons.csv
	python3 plot.py measures/measure_rw.csv
	python3 plot.py measures/measure_tas.csv -m3 measures/measure_ttas.csv measures/measure_backoff.csv
	python3 plot.py measures/measure_philo.csv -m3s measures/measure_philotas.csv measures/measure_philottas.csv
	python3 plot.py measures/measure_prodcons.csv -m3s measures/measure_prodconstas.csv measures/measure_prodconsttas.csv
	python3 plot.py measures/measure_rw.csv -m3s measures/measure_rwtas.csv measures/measure_rwttas.csv

tas: src/tas.c src/MyMutex/mytasmutex.c
	$(run)

ttas: src/ttas.c src/MyMutex/myttasmutex.c
	$(run)

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

backoff: src/BackOff/backoff.c src/MyMutex/mybackoffmutex.c
	$(run)

ttastest: src/ttastest.c
	$(run)

zip:
	zip -r Projet23-LINFO1252.zip src scripts measures headers plot.py experiments.sh Makefile README.md RAPPORT_LINFO1252

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
