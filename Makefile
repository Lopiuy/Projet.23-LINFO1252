

philo: philosophe.c
	gcc -o philo -Wall philosophe.c



zip:
	tar -zcvf prog.tar.gz philosphe.c rw.c prodcons.c Makefile README.md

clean:
	rm -f src/*.o
	rm -f philo
