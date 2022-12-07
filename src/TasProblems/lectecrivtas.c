#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include "../../headers/mytasmutex.h"
#include "../../headers/mysemtas.h"


#define Nw 640   // nombre total d'écriture
#define Nr 2560  // nombre total de lecture

int mutex_readcount = 0; // protège readcount
int mutex_writecount = 0; // protège writecount
int z = 0;

mysem_t* wsem; // accès exclusif à la database
mysem_t* rsem; // pour bloquer des readers

int readcount = 0;  // nombre de lecteurs actifs
int writecount = 0; // nombre d'écrivains actifs


void* writer(void* arg)
{
    int *stop = (int *) arg;    // nombre d'écriture à effectuer
    for (int i = 0; i < *stop; i++)
    {
        lock(&mutex_writecount);
        writecount++;   // on augmente le nombre d'écrivains actifs
        if (writecount==1)
        {
            mysem_wait(rsem);   // attend que le lecteur et fini de lire et empêche les suivants de lire (priorité aux écrivains)
        }
        unlock(&mutex_writecount);
        mysem_wait(wsem);

        // write database
        for (int i = 0; i < 10000; i++);

        mysem_post(wsem);
        lock(&mutex_writecount);
        writecount--;   // décrémente le nombre d'écrivains actifs
        if(writecount==0)       // si plus d'écrivains actifs
        {                       //  permettre au lecteurs de lire
            mysem_post(rsem);
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

        mysem_wait(rsem);   // un seul lecteur peu lire à la fois
        lock(&mutex_readcount);
        readcount++;    // on augmente le nombre de lecteurs actifs
        if (readcount==1)
        {
            mysem_wait(wsem);   // empecher les écrivains d'écrire
        }
        unlock(&mutex_readcount);
        mysem_post(rsem);   // libération du de l'écrivain ou du prochain lecteur en attente

        unlock(&z); //si on a un 2ème reader et un writer qui attende,
                    // on donne la priorité à l'écrivain. En faisant ceci, on empêche qu'un 2ème
                    // reader attende (vu qu'il doit attendre que z est unlock, et le post rsem est fait
                    // avant l'unlock -> écrivain va être libéré avant)

        // read database
        for (int i = 0; i < 10000; i++);

        lock(&mutex_readcount);
        readcount--;    // décrémente le nombre d'écrivains actifs
        if(readcount==0)    // si plus de lecteurs actifs
        {                   //  permettre au écrivains d'écrire
            mysem_post(wsem);
        }
        unlock(&mutex_readcount);
    }
    free(stop);
    return NULL;
}

int main(int argc, char *argv[]){

    if (argc < 3){
        perror("Invalid arguments was given");
        return -1;
    }

    int nwriters = atoi(argv[1]);
    int nreaders = atoi(argv[2]);

    pthread_t writers[nwriters];
    pthread_t readers[nreaders];

    wsem = mysem_init(1);
    if(wsem == NULL){
        perror("sem init of wsem failed with error");
        return -1;
    }
    rsem = mysem_init(1);
    if(rsem == NULL){
        perror("sem init of rsem failed with error");
        return -1;
    }

    for (int i = 0; i < nwriters; i++) {
        
        int p = Nw / nwriters;      // répartition de la charge de travail des écrivains
        if (i == nwriters-1){p += Nw % nwriters;}
        int *param = (int *) malloc(sizeof(int));
        memcpy(param, &p, sizeof(int));

        if (pthread_create(&writers[i], NULL, &writer, (void *) param) != 0){
            perror("Writer thread creation failed");
            return -1;
        }
    }

    for (int i = 0; i < nreaders; i++) {
        
        int p = Nr / nreaders;      // répartition de la charge de travail des lecteurs
        if (i == nreaders-1){p += Nr % nreaders;}
        int* param = (int *) malloc(sizeof(int));
        memcpy(param, &p, sizeof(int));

        if (pthread_create(&readers[i], NULL, &reader, (void *) param) != 0){
            perror("Reader thread creation failed");
            return -1;
        }
    }
    
    for (int i = 0; i < nwriters; i++) {
        if (pthread_join(writers[i], NULL) != 0){
            perror("Writer thread join failed");
            return -1;
        }
    }

    for (int i = 0; i < nreaders; i++) {
        if (pthread_join(readers[i], NULL) != 0) {
            perror("Reader thread join failed");
            return -1;
        }
    }

    int err = mysem_destroy(wsem);
    if(err != 0){
        perror("Semaphore wsem destroy failed with error");
        return -1;
    }
    err = mysem_destroy(rsem);
    if(err != 0){
        perror("Semaphore rsem destroy failed with error");
        return -1;
    }


    return EXIT_SUCCESS;
}