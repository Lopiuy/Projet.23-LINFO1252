#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define PHILOSOPHES 3

pthread_t phil[PHILOSOPHES];
pthread_mutex_t baguette[PHILOSOPHES];

void mange(int id) {
    printf("Philosophe [%d] mange\n",id);
}

void* philosophe( void* arg ){
    int *id=(int *) arg;
    int left = *id;
    int right = (left + 1) % PHILOSOPHES;
    while(true){
        // philosophe pense
        if(left<right){
            pthread_mutex_lock(&baguette[left]);
            pthread_mutex_lock(&baguette[right]);
        }else{
            pthread_mutex_lock(&baguette[right]);
            pthread_mutex_lock(&baguette[left]);
        }
        mange(*id);
        pthread_mutex_unlock(&baguette[left]);
        pthread_mutex_unlock(&baguette[right]);
    }
    return (NULL);
}