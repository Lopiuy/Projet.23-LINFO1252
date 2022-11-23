

philo: philosophe.c
	gcc -o prog -Wall main.c


clean:
	rm -f src/*.o
	rm -f philo