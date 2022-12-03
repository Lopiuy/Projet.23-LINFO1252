#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define N 6400
int min = 1000000;
int max = 0;

int verrou = 0;

int testAndSet(int* verrou,int a){
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

void lock(int *verrou){
    int i = 0;
    while (testAndSet(verrou,1)){
        while(*verrou){
        }
        i++;
    }
    if(i<min){min = i;}
    if(i>max){max = i;}
}


void unlock(int *verrou) {
    testAndSet(verrou,0);
}

void *func(void *param){
    int stop = *((int *) param);
    for (int i = 0; i < stop; i++) {
        lock(&verrou);
        // critical section
        for (int i = 0; i < 10000; i++);
        unlock(&verrou);
    }
    return NULL;
}

int main(int argc, char *argv[]){

    if (argc != 2) {
        fprintf(stderr, "Error: %s\n", "Invalid arguments was given");
        exit(EXIT_FAILURE);
    }

    int nthreads = atoi(argv[1]);
    pthread_t threads[nthreads];

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
    printf("%d,%d,",min,max);
    printf("%d\n",nthreads);

    return EXIT_SUCCESS;
}