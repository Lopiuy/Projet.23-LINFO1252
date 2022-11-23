#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t mutex;
sem_t db;  // accès à la db
int readcount = 0; // nombre de readers
sem_init(&db, 0, 1);

pthread_mutex_t mutex_readcount; //Protège readcount
pthread_mutex_t mutex_writecount; //Protège writecount

pthread_mutex_t z;

sem_t wsem; //Accès exclusif à la db
sem_t rsem; //Pour bloquer des readers

int readCount = 0;
int writeCount = 0;

sem_init(&wsem,0,1);
sem_init(&rsem,0,1);


void writer(void)
{
    while(true)
    {
        prepare_data();
        pthread_mutex_lock(&mutex_writecount);
        writeCount++;
        if (writeCount==1)
        {
            sem_wait(&rsem);
        }
        pthread_mutex_unlock(&mutex_writecount);

        sem_wait(&wsem);
        write_database();
        sem_post(&wsem);

        pthread_mutex_lock(&mutex_writecount);
        writeCount--;
        if(writeCount==0)
        {
            sem_post(&rsem);
        }
        pthread_mutex_unlock(&mutex_writecount);
    }
}

void reader(void)
{
    while(true)
    {
        pthread_mutex_lock(&z);

        sem_wait(&rsem); //un seul reader à la fois
        pthread_mutex_lock(&mutex_readcount);
        readcount++;
        if (readCount==1)
        {
            sem_wait(&wsem);
        }
        pthread_mutex_unlock(&mutex_readcount);
        sem_post(&rsem); //libération du prochain reader

        pthread_mutex_unlock(&z); //si on a un 2ème reader et un writer qui attende wsem (sem_wait(&rsem)),
        // il faut donner la priorité au writer. En faisant ceci, on empêche qu'un 2ème
        // reader attende (vu qu'il doit attendre que z est unlock, et le post rsem est fait
        // avant l'unlock -> writer va être libéré avant)

        read_database();

        pthread_mutex_lock(&mutex_readcount);
        readcount--;
        if(readCount==0)
        {
            sem_post(&wsem);
        }
        pthread_mutex_unlock(&mutex_readcount);

        process_data();
    }
}