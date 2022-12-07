#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define Nw 640   // nombre total d'écriture
#define Nr 2560  // nombre total de lecture

pthread_mutex_t mutex_readcount;  // protège readcount
pthread_mutex_t mutex_writecount; // protège writecount
pthread_mutex_t z;

sem_t wsem;   // accès exclusif à la database
sem_t rsem;   // pour bloquer des readers

int readcount = 0;  // nombre de lecteurs actifs
int writecount = 0; // nombre d'écrivains actifs


void* writer(void* arg)
{
    int *stop = (int *) arg;    // nombre d'écriture à effectuer
    for (int i = 0; i < *stop; i++){
        if(pthread_mutex_lock(&mutex_writecount)){
            perror("writer mutex failed with error");
            exit(-1);
        }
        writecount++;   // on augmente le nombre d'écrivains actifs
        if (writecount==1){
            if (sem_wait(&rsem)){   // attend que le lecteur et fini de lire et empêche les suivants de lire (priorité aux écrivains)
                perror("sem_wait failed with error");
                exit(-1);
            }
        }
        if(pthread_mutex_unlock(&mutex_writecount)){
            perror("writer mutex failed with error");
            exit(-1);
        }
        if(sem_wait(&wsem)){    // attente d'accès à la database
            perror("sem_wait failed with error");
            exit(-1);
        }

        // write database
        for (int i = 0; i < 10000; i++);

        if(sem_post(&wsem)){
            perror("sem_post failed with error");
            exit(-1);
        }
        if(pthread_mutex_lock(&mutex_writecount)){
            perror("writer mutex failed with error");
            exit(-1);
        }
        writecount--;   // décrémente le nombre d'écrivains actifs

        if(writecount==0){          // si plus d'écrivains actifs
            if(sem_post(&rsem)){    //  permettre au lecteurs de lire
                perror("sem_post failed with error");
                exit(-1);
            }
        }
        if(pthread_mutex_unlock(&mutex_writecount)){
            perror("writer mutex failed with error");
            exit(-1);
        }
    }
    free(stop);
    return NULL;
}

void* reader(void* arg)
{
    int *stop = (int *) arg;
    for (int i = 0; i < *stop; i++)
    {
        if(pthread_mutex_lock(&z)){
            perror("z mutex failed with error");
            exit(-1);
        }

        if(sem_wait(&rsem)){    // un seul lecteur peu lire à la fois
            perror("sem_wait failed with error");
            exit(-1);
        }
        if(pthread_mutex_lock(&mutex_readcount)){
            perror("reader mutex failed with error");
            exit(-1);
        }
        readcount++;    // on augmente le nombre de lecteurs actifs
        if (readcount==1)
        {
            if(sem_wait(&wsem)){    // empecher les écrivains d'écrire
                perror("sem_wait failed with error");
                exit(-1);
            }
        }
        if(pthread_mutex_unlock(&mutex_readcount)){
            perror("reader mutex failed with error");
            exit(-1);
        }
        if(sem_post(&rsem)) {  // libération du de l'écrivain ou du prochain lecteur en attente
            perror("sem_post failed with error");
            exit(-1);
        }

        if(pthread_mutex_unlock(&z)) {              // si on a un 2ème reader et un writer qui attende rsem,
            perror("z mutex failed with error");    //  on donne la priorité à l'écrivain. En faisant ceci, on empêche qu'un 2ème
            exit(-1);                               //  reader attende (vu qu'il doit attendre que z est unlock, et le post rsem est fait
        }                                           //  avant l'unlock -> écrivain va être libéré avant)

        // read database
        for (int i = 0; i < 10000; i++);

        if(pthread_mutex_lock(&mutex_readcount)){
            perror("reader mutex failed with error");
            exit(-1);
        }
        readcount--;    // décrémente le nombre d'écrivains actifs
        if(readcount==0){           // si plus de lecteurs actifs
            if(sem_post(&wsem)){    //  permettre au écrivains d'écrire
                perror("sem_post failed with error");
                exit(-1);
            }
        }
        if(pthread_mutex_unlock(&mutex_readcount)){
            perror("reader mutex failed with error");
            exit(-1);
        }
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

    if (pthread_mutex_init(&mutex_readcount, NULL) || pthread_mutex_init(&mutex_writecount, NULL) || pthread_mutex_init(&z, NULL)){
        perror("Mutex initialization failed");
        return -1;
    }

    if (sem_init(&wsem,0,1) == -1 || sem_init(&rsem,0,1) == -1) {
        perror("Semaphore initialization failed");
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

    if (pthread_mutex_destroy(&mutex_readcount) || pthread_mutex_destroy(&mutex_writecount) || pthread_mutex_destroy(&z) || sem_destroy(&rsem) || sem_destroy(&wsem)){
        perror("Destroy failed");
        return -1;
    }

    return 0;
}