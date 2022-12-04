#ifndef SEM_H
#define SEM_H

#include "../headers/queue.h"

typedef struct cond cond_t;
struct cond {
    queue_t *q; // queue of sleeping threads
    int mx; // mutex to protect the queue
};

typedef struct {
    int verrou; // = mutex
    int value; // value of the semaphore
    int wakeup; // number of threads that can be woken up
    cond_t *cond; //condition variable to suspend threads
} mysem_t;

void lock(int *verrou);

void unlock(int *verrou);

cond_t *cond_init(void);

void cond_wait(cond_t *cv, int *verrou);

void cond_signal(cond_t *cv);

mysem_t *mysem_init(int value);

void mysem_wait(mysem_t *semaphore);

void mysem_post(mysem_t *semaphore);

int mysem_destroy(mysem_t *semaphore);

#endif