#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/myttasmutex.h"

int* sticks_mutex;  // protège l'accès aux baguettes
int nb_philosophers = 0;    // nombre de philosophes
int nb_sticks = 0;          // nombre de baguettes


void eat() { // simule un philosophe qui mange
    return;
}

void think(){ // simule un philosophe qui pense
    return;
}

void* philosopher(void* args){
    int *id = (int*)args;                   // chiffre d'identité du philosophe
    int left = *id;                         // id de sa baguette de gauche
    int right = (left + 1) % nb_sticks;     // id de se baguette de droite
    int i = 0;
    while(i < 100000){                      // chaque philosophe fais 100000 cyles pensé-mangé
        think();
        if(left<right){
            lock(&sticks_mutex[left]);
            lock(&sticks_mutex[right]);
        }else{
            lock(&sticks_mutex[right]);
            lock(&sticks_mutex[left]);
        }
        eat();
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

    if(nb_sticks == 1){nb_sticks++;}    // cas avec 1 philosophe

    pthread_t philosophers[nb_philosophers];
    sticks_mutex = (int*) calloc(nb_sticks, sizeof(int));


    for(int i = 0; i < nb_philosophers; i++){   // lancer les philosophes
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