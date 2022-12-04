#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/myttasmutex.h"
#include "../headers/mysemttas.h"

int* buffer;
int buff_mutex = 0;
mysem_t* sem_full;
mysem_t* sem_empty;
int buf_in_index = 0;
int buf_out_index = 0;


void* producer_func(void* arg){
    int* iter = (int*)arg;
    for(int i = 0; i < *iter; i++) {
        for (int j = 0; j < 10000; j++) {               //simulate production
        }

        mysem_wait(sem_empty);
        lock(&buff_mutex);

        buffer[buf_in_index] = 2;
        buf_in_index = (buf_in_index + 1) % 8;

        unlock(&buff_mutex);
        mysem_post(sem_full);
    }
    free(iter);
    return (NULL);

}

void* consumer_func(void* arg){
    int* iter = (int*)arg;
    for(int i = 0; i < *iter; i++){
        mysem_wait(sem_full);
        lock(&buff_mutex);

        buffer[buf_out_index] = 0;
        buf_out_index = (buf_out_index + 1) % 8;

        unlock(&buff_mutex);
        mysem_post(sem_empty);

        for(int j = 0; j < 10000; j++){                     //simulate consumption
        }
    }
    free(iter);
    return (NULL);
}


int main(int argc, char * argv[]){

    if(argc < 3){
        perror("Please enter number of consumer threads and producer threads");
        return -1;
    }

    int nb_consumers = atoi(argv[1]);
    int nb_producers = atoi(argv[2]);


    buffer = (int*)malloc(8*sizeof(int));
    if(buffer == NULL){
        perror("Buffer allocation failed with error");
        return -1;
    }


    sem_empty = mysem_init(8);
    if(sem_empty == NULL){
        perror("sem init of sem_empty failed with error");
        return -1;
    }
    sem_full = mysem_init(0);
    if(sem_full == NULL){
        perror("sem init of sem_full failed with error");
        return -1;
    }


    pthread_t consumers[nb_consumers];
    pthread_t producers[nb_producers];


    int iter = 8192/nb_consumers;
    for(int i = 0; i < nb_consumers; i++){
        if(i == nb_consumers-1){
            iter += 8192%nb_consumers;
        }
        int* param = (int*)malloc(sizeof(int));
        if( param == NULL){
            perror("Number of consumer iteration allocation failed with error");
            return -1;
        }
        memcpy(param,&iter,sizeof(int));
        if (pthread_create(&consumers[i], NULL, &consumer_func, (void *) param) != 0){
            perror("Creation of consumer thread failed.");
            return -1;
        }
    }


    iter = 8192/nb_producers;
    for(int i = 0; i < nb_producers; i++){
        if(i == nb_producers-1){
            iter += 8192%nb_producers;
        }
        int* param = (int*)malloc(sizeof(int));
        if( param == NULL){
            perror("Number of producer iteration allocation failed with error");
            return -1;
        }
        memcpy(param,&iter,sizeof(int));
        if (pthread_create(&producers[i], NULL, &producer_func, (void *) param) != 0){
            perror("Creation of consumer thread failed.");
            return -1;
        }
    }


    for(int i = 0; i < nb_consumers; i++){
        if (pthread_join(consumers[i], NULL) != 0){
            perror("Creation of consumer thread failed.");
            return -1;
        }
    }


    for(int i = 0; i < nb_producers; i++){
        if (pthread_join(producers[i], NULL) != 0){
            perror("Creation of consumer thread failed.");
            return -1;
        }
    }


    int err = mysem_destroy(sem_empty);
    if(err != 0){
        perror("Semaphore sem_empty destroy failed with error");
        return -1;
    }
    err = mysem_destroy(sem_full);
    if(err != 0){
        perror("Semaphore sem_full destroy failed with error");
        return -1;
    }
    free(buffer);
    return 0;


}