#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


void mange(int id) {
    printf("Philosophe [%d] mange\n",id);
}

void pense(int id){
    printf("Philosophe [%d] pense\n",id);
}

void* philosophe(void* arg){
    int *id=(int *) arg;
    int left = *id;
    int right = (left + 1) % NB_PHIL;
    int i = 0;
    while(i < 100000){
        pense(*id);
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
        i++;
    }
    return (NULL);
}

int main(int argc, char * argv[]){

    if(argc == 1){
        printf("%s","Please enter number of philosophes");
        return 0;
    }
    int* nb_philosophe = (int*)argv[0];

    pthread_t phil[*nb_philosophe];
    pthread_mutex_t baguette[*nb_philosophe];

#define NB_PHIL *nb_philosophe

    for(int i = 0; i < *nb_philosophe; i++){
        if (pthread_create(&phil[i], NULL, &philosophe, (void *) &i) != 0){
            perror("Creation of worker thread failed.");
        }
    }
}