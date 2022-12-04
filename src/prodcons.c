#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

int* buffer;
pthread_mutex_t buff_mutex;
sem_t sem_full;
sem_t sem_empty;
int buf_in_index = 0;
int buf_out_index = 0;


void* producer_func(void* arg){
    int* iter = (int*)arg;
    for(int i = 0; i < *iter; i++) {
        for (int j = 0; j < 10000; j++) {               //simulate production
        }

        if (sem_wait(&sem_empty)){
            perror("sem_wait failed with error");
            exit(-1);
        }
        if (pthread_mutex_lock(&buff_mutex)){
            perror("Buffer mutex failed with error");
            exit(-1);
        }

        buffer[buf_in_index] = 2;
        buf_in_index = (buf_in_index + 1) % 8;

        if (pthread_mutex_unlock(&buff_mutex)){
            perror("Buffer mutex failed with error");
            exit(-1);
        }
        if (sem_post(&sem_full)){
            perror("sem_post failed with error");
            exit(-1);
        }
    }
    free(iter);
    return (NULL);

}

void* consumer_func(void* arg){
    int* iter = (int*)arg;
    for(int i = 0; i < *iter; i++){
        if(sem_wait(&sem_full)){
            perror("sem_wait failed with error");
            exit(1);
        }
        if(pthread_mutex_lock(&buff_mutex)){perror("Buffer mutex failed with error");
            exit(1);
        }

        buffer[buf_out_index] = 0;
        buf_out_index = (buf_out_index + 1) % 8;

        if(pthread_mutex_unlock(&buff_mutex)){
            perror("Buffer mutex failed with error");
            exit(1);
        }
        if(sem_post(&sem_empty)){
            perror("sem_post failed with error");
            exit(1);
        }

        for(int j = 0; j < 10000; j++){                     //simulate consumption
        }
    }
    free(iter);
    return (NULL);
}


int main(int argc, char * argv[]){

    //ptet faire des nb de threads par default??
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


    if(pthread_mutex_init(&buff_mutex,NULL)){
        perror("Mutex initailisation failed with error");
        return -1;
    }


    if(sem_init(&sem_empty,0,8) != 0){
        perror("Creation of semaphore failed.");
        return -1;
    }
    if(sem_init(&sem_full,0,0) != 0){
        perror("Creation of semaphore failed.");
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


    if(sem_destroy(&sem_empty)){
        perror("Semaphore destroy failed with error");
        return -1;
    }
    if(sem_destroy(&sem_full)){
        perror("Semaphore destroy failed with error");
        return -1;
    }
    if(pthread_mutex_destroy(&buff_mutex)){
        perror("Mutex destroy failed with error");
        return -1;
    }
    free(buffer);
    return 0;


}