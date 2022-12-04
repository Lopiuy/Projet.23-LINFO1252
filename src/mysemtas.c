#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../headers/mysemtas.h"

cond_t *cond_init(void) {

    cond_t *cond = (cond_t *) malloc(sizeof(cond_t));
    if (cond == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    int v = 0;
    cond->mx = v;

    queue_t *q = (queue_t *) malloc(sizeof(queue_t));
    if (q == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    q->first = NULL;
    q->last = NULL;
    cond->q = q;

    return cond;
}

void cond_wait(cond_t *cv, int *verrou) {

    lock(&(cv->mx)); // make the queue thread safe
    int id = 0;
    enqueue(cv->q, &id);
    lock(&id);
    unlock(&(cv->mx));

    unlock(verrou);
    lock(&id); // dummy lock to simulate sleeping
    unlock(&id);
    lock(verrou);
}

void cond_signal(cond_t *cv) {

    lock(&(cv->mx));
    int *id = dequeue(cv->q);
    unlock(&(cv->mx));

    if (id != NULL) {
        unlock(id);
    }
    // if the queue is empty, do nothing
}

mysem_t *mysem_init(int value) {

    mysem_t *semaphore = (mysem_t *) malloc(sizeof(mysem_t));
    if (semaphore == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    semaphore->value = value;
    semaphore->wakeup = 0;

    int v = 0;
    semaphore->verrou = v;

    cond_t *cond = cond_init();
    if (cond == NULL) {
        return NULL;
    }
    semaphore->cond = cond;

    return semaphore;
}

void mysem_wait(mysem_t *semaphore) {

    lock(&(semaphore->verrou));
    semaphore->value--;

    if (semaphore->value < 0) {
        cond_wait(semaphore->cond, &(semaphore->verrou));
        while(semaphore->wakeup < 0); // loop until wake up
        semaphore->wakeup--; // consume one wake up
    }
    unlock(&(semaphore->verrou));
}

void mysem_post(mysem_t *semaphore) {

    lock(&(semaphore->verrou));
    semaphore->value++;

    if (semaphore->value <= 0) {
        semaphore->wakeup++;
        cond_signal(semaphore->cond);
    }
    unlock(&(semaphore->verrou));
}

int mysem_destroy(mysem_t *semaphore) {

    int err;
    if (semaphore == NULL) {
        err = EINVAL;
    } else {
        if (semaphore->value < 0) {
            unlock(&(semaphore->verrou));
            err = EBUSY;
        } else {
            if (semaphore->cond == NULL) {
                free(semaphore);
                err = EINVAL;
            } else {
                if (semaphore->cond->q == NULL) {
                    free(semaphore->cond);
                    free(semaphore);
                    err = EINVAL;
                } else {
                    free(semaphore->cond->q);
                    free(semaphore->cond);
                    free(semaphore);
                    err = 0;
                }
            }
        }
    }
    if (err != 0) {
        errno = err;
    }
    return err;
}

