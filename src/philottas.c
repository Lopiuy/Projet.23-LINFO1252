#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/myttasmutex.h"

int* baguette_mutex;
int nb_philosophs = 0;
int nb_baguettes = 0;

void eat(int id) {
    //fprintf(stderr,"Philosophe [%d] mange\n",id);
    return;
}

void think(int id){
    //fprintf(stderr,"Philosophe [%d] pense\n",id);
    return;
}

void* philosophe(void* args){
    //fprintf(stderr,"%s\n","DEBUT");
    int *id = (int*)args;
    int left = *id;
    int right = (left + 1) % nb_baguettes;
    int i = 0;
    while(i < 100000){  //100000
        think(*id);
        if(left<right){
            lock(&baguette_mutex[left]);
            lock(&baguette_mutex[right]);
        }else{
            lock(&baguette_mutex[right]);
            lock(&baguette_mutex[left]);
        }
        eat(*id);
        unlock(&baguette_mutex[left]);
        unlock(&baguette_mutex[right]);
        i++;
    }
    return (NULL);
}

int main(int argc, char * argv[]){

    if(argc != 2){
        fprintf(stderr,"%s","Please enter number of philosophes");
        return -1;
    }
    nb_philosophs = atoi(argv[1]);
    nb_baguettes = atoi(argv[1]);

    if(nb_baguettes == 1){nb_baguettes++;}

    pthread_t phil[nb_philosophs];
    baguette_mutex = (int*) calloc(nb_baguettes, sizeof(int));

    for(int i = 0; i < nb_philosophs; i++){
        int* id = (int*)malloc(sizeof(int));
        memcpy(id,&i,sizeof(int));
        if (pthread_create(&phil[i], NULL, &philosophe, (void *) id) != 0){
            perror("Creation of philosopher thread failed.");
        }
    }

    for (int i = 0; i < nb_philosophs; i++){
        if (pthread_join(phil[i], NULL) != 0){
            perror("Try to join philosopher thread failed.");
        }
    }

}