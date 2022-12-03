#ifndef QUEUE_H
#define QUEUE_H

typedef struct node node_t;
struct node {
    int *id;
    node_t *next;
};

typedef struct {
    node_t *first;
    node_t *last;
} queue_t;

int *dequeue(queue_t *q);

void enqueue(queue_t *q, int *id);

#endif