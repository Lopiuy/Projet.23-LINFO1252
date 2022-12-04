#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "../headers/mybackoffmutex.h"

#define N 6400

int max;
int verrou = 0;


void *func(void *param){
    int stop = *((int *) param);
    for (int i = 0; i < stop; i++) {
        lock(&verrou,i%10,max);
        // critical section
        for (int j = 0; j < 10000; j++);
        unlock(&verrou);
    }
    return NULL;
}

int main(int argc, char *argv[]){

    if (argc < 2) {
        perror("Invalid arguments was given");
        return -1;
    }

    int nthreads = atoi(argv[1]);
    pthread_t threads[nthreads];

    max = 2500*pow(nthreads,-0.55);

    for (int i = 0; i < nthreads ; i++) {

        int p = N/nthreads;
        if (i == nthreads - 1){p += N % nthreads;}
        int *param = (int *) malloc(sizeof(int));
        memcpy(param, &p, sizeof(int));

        if (0 != pthread_create(&threads[i], NULL, &func, (void *) param)) {
            fprintf(stderr, "Error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nthreads ; i++) {
        if (0 != pthread_join(threads[i], NULL)) {
            fprintf(stderr, "Error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}