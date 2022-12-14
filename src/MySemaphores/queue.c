#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "../../headers/queue.h"

// opération classique de dequeue sur une file
int *dequeue(queue_t *q) {
    if (q->first == NULL) {
        return NULL;
    }
    int *id = q->first->id;
    //node_t *tmp = q->first;
    q->first = q->first->next;
    //free(tmp);
    return id;
}

// opération classique de enqueue sur une file
// le second argument est l'identifiant du verrou inhérent à un thread endormi
void enqueue(queue_t *q, int *id) { 
    node_t *newNode = (node_t *) malloc(sizeof(node_t));
    if (newNode == NULL) {
        fprintf(stderr, "Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    newNode->id = id;
    newNode->next = NULL;
    if (q->last != NULL) q->last->next = newNode;
    q->last = newNode;
    if (q->first == NULL) q->first = newNode;
}