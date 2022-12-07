#ifndef SEM_H
#define SEM_H

#include "../headers/queue.h"
#include "../headers/myttasmutex.h"

typedef struct cond cond_t;
struct cond {
    queue_t *q; // file de threads endormis
    int mx; // mutex pour protéger la file
};

typedef struct {
    int verrou; // = mutex
    int value; // valeur de la semaphore
    int wakeup; // nombre de threads qui peuvent être réveillés
    cond_t *cond; // variable conditionnelle pour suspendre les threads
} mysem_t;

cond_t *cond_init(void);

void cond_wait(cond_t *cv, int *verrou);

void cond_signal(cond_t *cv);

mysem_t *mysem_init(int value);

void mysem_wait(mysem_t *semaphore);

void mysem_post(mysem_t *semaphore);

int mysem_destroy(mysem_t *semaphore);

#endif