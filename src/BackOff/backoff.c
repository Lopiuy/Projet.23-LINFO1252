#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/mybackoffmutex.h"

#define N 6400

int min;
int max;
int verrou = 0;


void *func(void *param){
    int stop = *((int *) param);
    for (int i = 0; i < stop; i++) {
        lock(&verrou,(i%min)+1,max);
        // critical section
        for (int j = 0; j < 10000; j++);
        unlock(&verrou);
    }
    return NULL;
}

int main(int argc, char *argv[]){

    if (argc < 3) {
        perror("Invalid arguments was given");
        return -1;
    }

    int nthreads = atoi(argv[1]);
    min = atoi(argv[2]);
    if(min == 0){
        min++;
    }
    max = atoi(argv[3]);
    pthread_t threads[nthreads];


    for (int i = 0; i < nthreads ; i++) {

        int p = N/nthreads;
        if (i == nthreads - 1){p += N % nthreads;}
        int *param = (int *) malloc(sizeof(int));
        if(param == NULL){
            perror("Allocation failed");
            return -1;
        }
        memcpy(param, &p, sizeof(int));

        if (0 != pthread_create(&threads[i], NULL, &func, (void *) param)) {
            perror("Thread creation failed");
            return -1;
        }
    }

    for (int i = 0; i < nthreads ; i++) {
        if (0 != pthread_join(threads[i], NULL)) {
            perror("Thread joining failed");
            return -1;
        }
    }

    return 0;
}