#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <errno.h>

#define Nw 640 // total number of writings // to change -> 640 (x128)
#define Nr 2560 // total number of readings // to change -> 2560 (x128)

pthread_mutex_t mutex_readcount; //Protège readcount
pthread_mutex_t mutex_writecount; //Protège writecount
pthread_mutex_t z;

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
    // int *stop = (int *) arg;
    for (int i = 0; i < *((args_t *) arg)->stop; i++) // to change
    {
        pthread_mutex_lock(&mutex_writecount);
        writecount++;
        if (writecount==1)
        {
            sem_wait(&rsem);
        }
        pthread_mutex_unlock(&mutex_writecount);
        sem_wait(&wsem);

        // write database
        for (int i = 0; i < 10000; i++);
        //printf("thread %d has written\n", *((args_t *) arg)->id);

        sem_post(&wsem);
        pthread_mutex_lock(&mutex_writecount);
        writecount--;
        if(writecount==0)
        {
            sem_post(&rsem);
        }
        pthread_mutex_unlock(&mutex_writecount);
    }
    return NULL;
}

void* reader(void* arg)
{
    // int *stop = (int *) arg;
    for (int i = 0; i < *((args_t *) arg)->stop; i++) // to change
    {
        pthread_mutex_lock(&z);

        sem_wait(&rsem); //un seul reader à la fois
        pthread_mutex_lock(&mutex_readcount);
        readcount++;
        if (readcount==1)
        {
            sem_wait(&wsem);
        }
        pthread_mutex_unlock(&mutex_readcount);
        sem_post(&rsem); //libération du prochain reader

        pthread_mutex_unlock(&z); //si on a un 2ème reader et un writer qui attende wsem (sem_wait(&rsem)),
        // il faut donner la priorité au writer. En faisant ceci, on empêche qu'un 2ème
        // reader attende (vu qu'il doit attendre que z est unlock, et le post rsem est fait
        // avant l'unlock -> writer va être libéré avant)

        // read database
        for (int i = 0; i < 10000; i++);
        //printf("thread %d has read\n", *((args_t *) arg)->id);

        pthread_mutex_lock(&mutex_readcount);
        readcount--;
        if(readcount==0)
        {
            sem_post(&wsem);
        }
        pthread_mutex_unlock(&mutex_readcount);
    }
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

    if (pthread_mutex_init(&mutex_readcount, NULL) || pthread_mutex_init(&mutex_writecount, NULL) || pthread_mutex_init(&z, NULL)){
        fprintf(stderr, "Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (sem_init(&wsem,0,1) == -1 || sem_init(&rsem,0,1) == -1) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nwriters; i++) {
        
        int p = Nw / nwriters;
        if (i == nwriters-1){p += Nw % nwriters;}
        int *param = (int *) malloc(sizeof(int));
        memcpy(param, &p, sizeof(int));

        int *id = (int *) malloc(sizeof(int)); // to delete
        memcpy(id, &i, sizeof(int));

        args_t *arg = (args_t *) malloc(sizeof(args_t)); // to delete
        arg->stop = param;
        arg->id = id;

        if (pthread_create(&writers[i], NULL, &writer, (void *) arg) != 0){ // to change
            fprintf(stderr, "Error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < nreaders; i++) {
        
        int p = Nr / nreaders;
        if (i == nreaders-1){p += Nr % nreaders;}
        int* param = (int *) malloc(sizeof(int));
        memcpy(param, &p, sizeof(int));

        int *id = (int *) malloc(sizeof(int)); // to delete
        int j = i + nwriters;
        memcpy(id, &j, sizeof(int));

        args_t *arg = (args_t *) malloc(sizeof(args_t)); // to delete
        arg->stop = param;
        arg->id = id;

        if (pthread_create(&readers[i], NULL, &reader, (void *) arg) != 0){ // to change
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

    if (pthread_mutex_destroy(&mutex_readcount) || pthread_mutex_destroy(&mutex_writecount) || pthread_mutex_destroy(&z) || sem_destroy(&rsem) || sem_destroy(&wsem)){
        fprintf(stderr, "Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}