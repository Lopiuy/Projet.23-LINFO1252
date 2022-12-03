#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "mysem.h"

void lock(int *verrou) {
    asm (
    "enter:;"
        "movl %1, %%eax;"
        "xchg %%eax, %0;"
        "testl %%eax, %%eax;"
        "jnz enter;"
    :"=m"(*verrou)
    :"r"(1)
    :"%eax");
}

void unlock(int *verrou) {
    asm (
    "movl %1, %%eax;"
    "xchg %%eax, %0;"
    :"=m"(*verrou)
    :"r"(0) 
    :"%eax");
}

cond_t *cond_init(void) {

    cond_t *cond = (cond_t *) malloc(sizeof(cond_t));

    int v = 0;
    cond->mx = (int *) malloc(sizeof(int));
    cond->mx = &v;

    queue_t *q = (queue_t *) malloc(sizeof(queue_t));
    q->first = NULL;
    q->last = NULL;

    return cond;
}

void cond_wait(cond_t *cv, int *verrou) {

    lock(cv->mx); // make the queue thread safe
    int *id = (int *) calloc(1, sizeof(int));
    enqueue(cv->q, id);
    lock(id);
    unlock(cv->mx);

    unlock(verrou);
    lock(id);
    unlock(id);
    lock(verrou);
}

void cond_signal(cond_t *cv) {

    lock(cv->mx);
    int *id = dequeue(cv->q);
    unlock(cv->mx);

    if (id != NULL) {
        unlock(id);
    }
    // if the queue is empty, do nothing
}

mysem_t *mysem_init(int value) {

    mysem_t *semaphore = (mysem_t *) malloc(sizeof(mysem_t));
    semaphore->value = value;
    semaphore->wakeup = 0;

    int *verrou = (int *) malloc(sizeof(int));
    int v = 0;
    verrou = &v;
    semaphore->verrou = verrou;

    cond_t *cond = cond_init();
    semaphore->cond = cond;

    return semaphore;
}

void mysem_wait(mysem_t *semaphore) {

    lock(semaphore->verrou);
    semaphore->value--;

    if (semaphore->value < 0) {
        cond_wait(semaphore->cond, semaphore->verrou);
        while(semaphore->wakeup < 0); // loop until wake up
        semaphore->wakeup--; // consume one wake up
    }
    unlock(semaphore->verrou);
}

void mysem_post(mysem_t *semaphore) {

    printf("on est la hein\n");
    
    lock(semaphore->verrou);
    semaphore->value++;

    if (semaphore->value <= 0) {
        semaphore->wakeup++;
        cond_signal(semaphore->cond);
    }
    unlock(semaphore->verrou);
}

