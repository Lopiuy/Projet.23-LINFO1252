#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t* baguette_mutex;
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
            pthread_mutex_lock(&baguette_mutex[left]);
            pthread_mutex_lock(&baguette_mutex[right]);
        }else{
            pthread_mutex_lock(&baguette_mutex[right]);
            pthread_mutex_lock(&baguette_mutex[left]);
        }
        eat(*id);
        pthread_mutex_unlock(&baguette_mutex[left]);
        pthread_mutex_unlock(&baguette_mutex[right]);
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
    baguette_mutex = (pthread_mutex_t*) malloc(nb_baguettes * sizeof(pthread_mutex_t));

    for (int i = 0; i < nb_baguettes; i++) {
        if(pthread_mutex_init(&baguette_mutex[i],NULL)){perror("Mutex initailisation failed with error"); exit(1);}
    }

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

    for (int i = 0; i < nb_baguettes; i++) {
        if(pthread_mutex_destroy(&baguette_mutex[i])){perror("Mutex destroy failed with error"); exit(1);}
    }

}