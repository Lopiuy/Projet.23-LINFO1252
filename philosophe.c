#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>


void mange(int id) {
    //fprintf(stderr,"Philosophe [%d] mange\n",id);
}

void pense(int id){
    //fprintf(stderr,"Philosophe [%d] pense\n",id);
}

struct parse_arg{
    int id;
    int nb_philosophes;
    pthread_mutex_t* baguette;
};

void* philosophe(void* args){
    //fprintf(stderr,"%s","DEBUT");
    struct parse_arg* arg = (struct parse_arg*) args;
    int id = arg->id;
    int left = id;
    int right = (left + 1) % arg->nb_philosophes;
    int i = 0;
    while(i < 100000){
        pense(id);
        if(left<right){
            pthread_mutex_lock(&arg->baguette[left]);
            pthread_mutex_lock(&arg->baguette[right]);
        }else{
            pthread_mutex_lock(&arg->baguette[right]);
            pthread_mutex_lock(&arg->baguette[left]);
        }
        mange(id);
        pthread_mutex_unlock(&arg->baguette[left]);
        pthread_mutex_unlock(&arg->baguette[right]);
        i++;
    }
    return (NULL);
}

int main(int argc, char * argv[]){

    if(argc == 1){
        fprintf(stderr,"%s","Please enter number of philosophes");
        return -1;
    }
    struct parse_arg args = {.nb_philosophes = atoi(argv[1])};

    pthread_t phil[args.nb_philosophes];
    pthread_mutex_t baguette[args.nb_philosophes];

    //faire des mutex des variable globales pour pas devoir les mettre en argument


    args.baguette = (pthread_mutex_t*) malloc(args.nb_philosophes * sizeof(pthread_mutex_t));
    memcpy(args.baguette,baguette,args.nb_philosophes * sizeof(pthread_mutex_t));


    for(int i = 0; i < args.nb_philosophes; i++){
        args.id = i;
        struct parse_arg* param = (struct parse_arg*) malloc(sizeof(struct parse_arg));

        memcpy(param,&args,sizeof(struct parse_arg));
        if (pthread_create(&phil[i], NULL, &philosophe, (void *) param) != 0){
            perror("Creation of philosopher thread failed.");
        }
    }

    for (int i = 0; i < args.nb_philosophes; i++){
        if (pthread_join(phil[i], NULL) != 0){
            perror("Try to join philosopher thread failed.");
        }
    }

}