#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/myttasmutex.h"

int* sticks_mutex;
int nb_philosophers = 0;
int nb_sticks = 0;


void eat(int id) { //simulates an eating philosopher
    return;
}

void think(int id){ //simulates a thinking philosopher
    return;
}

void* philosopher(void* args){
    int *id = (int*)args;                   //philosopher identity number
    int left = *id;                         //id of his left stick
    int right = (left + 1) % nb_sticks;     //id of his right stick
    int i = 0;
    while(i < 100000){                      //each philosopher does 100000 thinking-eating cycles
        think(*id);
        if(left<right){
            lock(&sticks_mutex[left]);
            lock(&sticks_mutex[right]);
        }else{
            lock(&sticks_mutex[right]);
            lock(&sticks_mutex[left]);
        }
        eat(*id);
        unlock(&sticks_mutex[left]);
        unlock(&sticks_mutex[right]);
        i++;
    }
    free(id);
    return (NULL);
}

int main(int argc, char * argv[]){

    if(argc < 2){
        perror("Please enter number of philosophers");
        return -1;
    }
    nb_philosophers = atoi(argv[1]);
    nb_sticks = atoi(argv[1]);

    if(nb_sticks == 1){nb_sticks++;}                    //case with 1 philosopher

    pthread_t philosophers[nb_philosophers];
    sticks_mutex = (int*) calloc(nb_sticks, sizeof(int));


    for(int i = 0; i < nb_philosophers; i++){           //launching philosophers
        int* id = (int*)malloc(sizeof(int));
        if(id == NULL){
            perror("Mutexes malloc failed with error");
            return -1;
        }
        memcpy(id,&i,sizeof(int));
        if (pthread_create(&philosophers[i], NULL, &philosopher, (void *) id) != 0){
            perror("Creation of philosopher thread failed.");
            return -1;
        }
    }

    for (int i = 0; i < nb_philosophers; i++){
        if (pthread_join(philosophers[i], NULL) != 0){
            perror("Try to join philosopher thread failed.");
            return -1;
        }
    }
    free(sticks_mutex);
    return 0;

}