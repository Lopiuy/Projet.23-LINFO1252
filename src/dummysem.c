#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "../headers/mysem.h"

#define NTHREADS 2
//sem_t semaphore;
mysem_t *sem;

void *before(void * param) {
    // do something
    for(int j=0;j<1000000;j++) {
    }
    printf("first thread did his thing\n");
    mysem_post(sem);
    //sem_post(&semaphore);
    return(NULL);
}

void *after(void * param) {
    mysem_wait(sem);
    //sem_wait(&semaphore);
    // do something
    for(int j=0;j<1000000;j++) {
    }
    printf("and the second too\n");
    return(NULL);
}

int main (int argc, char *argv[])  {

    pthread_t thread[NTHREADS];
    void * (* func[])(void *)={before, after};
    int err;

    /*err=sem_init(&semaphore, 0,0);
    if(err!=0) {
      fprintf(stderr,"sem_init");
    }*/
    sem = mysem_init(0);

    for(int i=0;i<NTHREADS;i++) {
        err=pthread_create(&(thread[i]),NULL,func[i],NULL);
        if(err!=0) {
        fprintf(stderr,"pthread_create");
        }
    }

    for(int i=0;i<NTHREADS;i++) {
        err=pthread_join(thread[i],NULL);
        if(err!=0) {
        fprintf(stderr,"pthread_join");
        }
    }

    /*err=sem_destroy(&semaphore);
    if(err!=0) {
      fprintf(stderr,"sem_destroy");
    }*/
    return(EXIT_SUCCESS);
}