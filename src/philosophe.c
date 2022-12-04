#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t* sticks_mutex;
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
            pthread_mutex_lock(&sticks_mutex[left]);
            pthread_mutex_lock(&sticks_mutex[right]);
        }else{
            pthread_mutex_lock(&sticks_mutex[right]);
            pthread_mutex_lock(&sticks_mutex[left]);
        }
        eat(*id);
        pthread_mutex_unlock(&sticks_mutex[left]);
        pthread_mutex_unlock(&sticks_mutex[right]);
        i++;
    }
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
    sticks_mutex = (pthread_mutex_t*) malloc(nb_sticks * sizeof(pthread_mutex_t));

    for (int i = 0; i < nb_sticks; i++) {
        if(pthread_mutex_init(&sticks_mutex[i],NULL)){
            perror("Mutex initialisation failed with error");
            return -1;
        }
    }

    for(int i = 0; i < nb_philosophers; i++){           //launching philosophers
        int* id = (int*)malloc(sizeof(int));
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

    for (int i = 0; i < nb_sticks; i++) {
        if(pthread_mutex_destroy(&sticks_mutex[i])){
            perror("Mutex destroy failed with error");
            return -1;}
    }

}