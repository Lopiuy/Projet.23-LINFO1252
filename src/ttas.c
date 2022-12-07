#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../headers/myttasmutex.h"

#define N 6400 // nombre total de sections critiques à effectuer

int verrou = 0; // verrou par attente active

void *func(void *param){
    int stop = *((int *) param); // nombre de sections critiques par thread
    for (int i = 0; i < stop; i++) {
        lock(&verrou);
        // section critique
        for (int j = 0; j < 10000; j++);
        unlock(&verrou);
    }
    return NULL;
}

int main(int argc, char *argv[]){

    if (argc < 2) {
        fprintf(stderr, "Error: %s\n", "Invalid arguments was given");
        exit(EXIT_FAILURE);
    }

    int nthreads = atoi(argv[1]); // nombre de threads d'exécution
    pthread_t threads[nthreads];

    // lancement des threads
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

    // terminaison des threads
    for (int i = 0; i < nthreads ; i++) {
        if (0 != pthread_join(threads[i], NULL)) {
            fprintf(stderr, "Error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}