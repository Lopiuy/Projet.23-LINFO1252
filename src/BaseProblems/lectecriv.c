#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
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


void* writer(void* arg)
{
    int *stop = (int *) arg;
    for (int i = 0; i < *stop; i++)
    {
        if(pthread_mutex_lock(&mutex_writecount)){
            perror("writer mutex failed with error");
            exit(-1);
        }
        writecount++;
        if (writecount==1)
        {
            if (sem_wait(&rsem)){
                perror("sem_wait failed with error");
                exit(-1);
            }
        }
        if(pthread_mutex_unlock(&mutex_writecount)){
            perror("writer mutex failed with error");
            exit(-1);
        }
        if(sem_wait(&wsem)){
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
        writecount--;
        if(writecount==0)
        {
            if(sem_post(&rsem)){
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

        if(sem_wait(&rsem)){  //un seul reader à la fois
            perror("sem_wait failed with error");
            exit(-1);
        }
        if(pthread_mutex_lock(&mutex_readcount)){
            perror("reader mutex failed with error");
            exit(-1);
        }
        readcount++;
        if (readcount==1)
        {
            if(sem_wait(&wsem)){
                perror("sem_wait failed with error");
                exit(-1);
            }
        }
        if(pthread_mutex_unlock(&mutex_readcount)){
            perror("reader mutex failed with error");
            exit(-1);
        }
        if(sem_post(&rsem)) { //libération du prochain reader
            perror("sem_post failed with error");
            exit(-1);
        }

        if(pthread_mutex_unlock(&z)) {              //si on a un 2ème reader et un writer qui attende wsem (sem_wait(&rsem)),
            perror("z mutex failed with error");    // il faut donner la priorité au writer. En faisant ceci, on empêche qu'un 2ème
            exit(-1);                               // reader attende (vu qu'il doit attendre que z est unlock, et le post rsem est fait
        }                                           // avant l'unlock -> writer va être libéré avant)

        // read database
        for (int i = 0; i < 10000; i++);

        if(pthread_mutex_lock(&mutex_readcount)){
            perror("reader mutex failed with error");
            exit(-1);
        }
        readcount--;
        if(readcount==0)
        {
            if(sem_post(&wsem)){
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
        
        int p = Nw / nwriters;
        if (i == nwriters-1){p += Nw % nwriters;}
        int *param = (int *) malloc(sizeof(int));
        memcpy(param, &p, sizeof(int));

        if (pthread_create(&writers[i], NULL, &writer, (void *) param) != 0){
            perror("Writer thread creation failed");
            return -1;
        }
    }

    for (int i = 0; i < nreaders; i++) {
        
        int p = Nr / nreaders;
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