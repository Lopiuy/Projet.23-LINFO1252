#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include "../../headers/myttasmutex.h"


#define Nw 640 // total number of writings // to change -> 640 (x128)
#define Nr 2560 // total number of readings // to change -> 2560 (x128)

int mutex_readcount = 0; //Protège readcount
int mutex_writecount = 0; //Protège writecount
int z = 0;

sem_t wsem; //Accès exclusif à la db
sem_t rsem; //Pour bloquer des readers

int readcount = 0;
int writecount = 0;

typedef struct { // to delete
    int *stop;
    int *id;
} args_t;


void* writer(void* arg)
{
    int *stop = (int *) arg;
    for (int i = 0; i < *stop; i++)
    {
        lock(&mutex_writecount);
        writecount++;
        if (writecount==1)
        {
            sem_wait(&rsem);
        }
        unlock(&mutex_writecount);
        sem_wait(&wsem);

        // write database
        for (int i = 0; i < 10000; i++);

        sem_post(&wsem);
        lock(&mutex_writecount);
        writecount--;
        if(writecount==0)
        {
            sem_post(&rsem);
        }
        unlock(&mutex_writecount);
    }
    free(stop);
    return NULL;
}

void* reader(void* arg)
{
    int *stop = (int *) arg;
    for (int i = 0; i < *stop; i++)
    {
        lock(&z);

        sem_wait(&rsem); //un seul reader à la fois
        lock(&mutex_readcount);
        readcount++;
        if (readcount==1)
        {
            sem_wait(&wsem);
        }
        unlock(&mutex_readcount);
        sem_post(&rsem); //libération du prochain reader

        unlock(&z); //si on a un 2ème reader et un writer qui attende wsem (sem_wait(&rsem)),
        // il faut donner la priorité au writer. En faisant ceci, on empêche qu'un 2ème
        // reader attende (vu qu'il doit attendre que z est unlock, et le post rsem est fait
        // avant l'unlock -> writer va être libéré avant)

        // read database
        for (int i = 0; i < 10000; i++);

        lock(&mutex_readcount);
        readcount--;
        if(readcount==0)
        {
            sem_post(&wsem);
        }
        unlock(&mutex_readcount);
    }
    free(stop);
    return NULL;
}

int main(int argc, char *argv[]){

    if (argc != 3){
        fprintf(stderr, "Error: %s\n", "Invalid arguments was given");
        exit(EXIT_FAILURE);
    }

    int nwriters = atoi(argv[1]);
    int nreaders = atoi(argv[2]);

    pthread_t writers[nwriters];
    pthread_t readers[nreaders];

    if (sem_init(&wsem,0,1) == -1 || sem_init(&rsem,0,1) == -1) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nwriters; i++) {
        
        int p = Nw / nwriters;
        if (i == nwriters-1){p += Nw % nwriters;}
        int *param = (int *) malloc(sizeof(int));
        memcpy(param, &p, sizeof(int));

        if (pthread_create(&writers[i], NULL, &writer, (void *) param) != 0){ // to change
            fprintf(stderr, "Error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nreaders; i++) {
        
        int p = Nr / nreaders;
        if (i == nreaders-1){p += Nr % nreaders;}
        int* param = (int *) malloc(sizeof(int));
        memcpy(param, &p, sizeof(int));

        if (pthread_create(&readers[i], NULL, &reader, (void *) param) != 0){
            fprintf(stderr, "Error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    
    for (int i = 0; i < nwriters; i++) {
        if (pthread_join(writers[i], NULL) != 0){
            fprintf(stderr, "Error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nreaders; i++) {
        if (pthread_join(readers[i], NULL) != 0) {
            fprintf(stderr, "Error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}