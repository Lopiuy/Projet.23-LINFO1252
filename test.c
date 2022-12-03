#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "queue.h"

int main(){

    long ids[] = {1, 2, 3, 4, 5};
    queue_t *queue = (queue_t *) malloc(sizeof(queue_t));
    queue->first = NULL;
    queue->last = NULL;
    for (int i = 0; i < 5; i++) {
        long *ptr = (long *) malloc(sizeof(long));
        ptr = &ids[i];
        enqueue(queue, ptr);
    }

    node_t *runner = queue->first;

    for (int i = 0; i < 5; i++) {
        printf("value of the node : %ld\n", *(runner->id));
        runner = runner->next;
    }

    long *result = dequeue(queue);
    printf("poped value %ld\n", *result);
    long toadd = 0;
    long *toadd_ptr = (long *) malloc(sizeof(long));
    toadd_ptr = &toadd;
    enqueue(queue, toadd_ptr);
    printf("swapping one id...\n");

    runner = queue->first;
    for (int i = 0; i < 5; i++) {
        printf("value of the node : %ld\n", *(runner->id));
        runner = runner->next;
    }
    runner = queue->first;
    for (int i = 0; i < 4; i++) {
        dequeue(queue);
    }
    printf("after 4 dequeues\n");
    printf("value of the node : %ld\n", *(queue->first->id));
}
