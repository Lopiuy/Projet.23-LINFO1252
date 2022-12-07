#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define N 6400 // nombre de total de sections critiques à effectuer

// programme pour déterminer les valeurs minimale et maximale d'attente

int min = 1000000; // valeur d'attente minimale initiale
int max = 0; // valeur d'attente maximale initiale

int verrou = 0; // verrou TTAS

int testAndSettest(int* verrou,int a){
    int ret;
    asm (
        "movl %2, %%eax;"
        "xchg %%eax, %1;"
        "movl %%eax, %0"
    :"=r"(ret), "=m"(*verrou)
    :"r"(a)
    :"%eax");
    return ret;
}

void locktest(int *verrou){
    int i = 0;
    while (testAndSettest(verrou,1)){
        while(*verrou){
        }
        i++;
    }
    if(i<min){min = i;}
    if(i>max){max = i;}
}


void unlocktest(int *verrou) {
    testAndSettest(verrou,0);
}

void *func(void *param){
    int stop = *((int *) param); // nombre de sections critique à effectuer par thread
    for (int i = 0; i < stop; i++) {
        locktest(&verrou);
        // section critique
        for (int i = 0; i < 10000; i++);
        unlocktest(&verrou);
    }
    return NULL;
}

int main(int argc, char *argv[]){

    if (argc != 2) {
        fprintf(stderr, "Error: %s\n", "Invalid arguments was given");
        exit(EXIT_FAILURE);
    }

    int nthreads = atoi(argv[1]); // nombre de threads d'exécution
    pthread_t threads[nthreads];

    // lancement des threads
    for (int i = 0; i < nthreads ; i++) {

        int p = N/nthreads; // nombre de sections critique à effectuer par thread
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

    // impression des valeurs reccueillies
    printf("%d,%d,",min,max);
    printf("%d\n",nthreads);

    return EXIT_SUCCESS;
}