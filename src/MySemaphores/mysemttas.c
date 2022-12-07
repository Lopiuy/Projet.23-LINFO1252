#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "../../headers/mysemtas.h"

// initialisation de la variable conditionnelle
cond_t *cond_init(void) {

    cond_t *cond = (cond_t *) malloc(sizeof(cond_t));
    if (cond == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    int v = 0;
    cond->mx = v; // verrou qui protège la file de threads

    queue_t *q = (queue_t *) malloc(sizeof(queue_t));
    if (q == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    q->first = NULL; // premier thread dans la file
    q->last = NULL; // second thread dans la file
    cond->q = q; // la file de verrous (= id. des threads) est stockée dans la variable conditionnelle

    return cond;
}

// bloque les threads sur la variable conditionnelle
void cond_wait(cond_t *cv, int *verrou) {

    lock(&(cv->mx)); // verrouillage de la file
    int id = 0; // verrou exclusif à chaque thread endormi
    enqueue(cv->q, &id); // on ajoute le verrou dans la file
    lock(&id); // on endort le thread en le bloquant sur le verrou mis dans la file
    unlock(&(cv->mx)); // déverrouillage de la file

    unlock(verrou); // déverrouillage du verrou pour que les autres threads continuent leur exécution
    lock(&id); // verrouillage factice
    unlock(&id); // si le thread reprend le verrou c'est que ce dernier a été enlevé de la file quelque part :)
    lock(verrou); // reprise du verrou pour exécuter la suite de sem_wait
}

// envoie un signal aux threads bloqués sur la variable conditionnelle
void cond_signal(cond_t *cv) {

    lock(&(cv->mx)); // verrouillage de la file
    int *id = dequeue(cv->q); // on récupère le verrou d'un thread bloqué
    unlock(&(cv->mx)); // déverrouillage de la file

    if (id != NULL) {
        unlock(id); // débloquons ce threads
    }
    // si la queue est vide, on ne fait rien
}

// initialisation de la sémaphore
mysem_t *mysem_init(int value) {

    mysem_t *semaphore = (mysem_t *) malloc(sizeof(mysem_t));
    if (semaphore == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    semaphore->value = value; // valeur de la sémaphore initialisée à value
    semaphore->wakeup = 0; // aucun thread à réveiller pour l'instant

    int v = 0;
    semaphore->verrou = v; // verrou pour protéger l'accès à la valeur de la sémaphore

    cond_t *cond = cond_init(); // initialisation de la variable conditionnelle associée à la sémaphore
    if (cond == NULL) {
        return NULL;
    }
    semaphore->cond = cond;

    return semaphore;
}

// décrémente la valeur de la sémaphore, si elle est négative, 
// ajoute le thread appelant dans la file
void mysem_wait(mysem_t *semaphore) {

    lock(&(semaphore->verrou));
    semaphore->value--;

    if (semaphore->value < 0) {
        cond_wait(semaphore->cond, &(semaphore->verrou));
        while(semaphore->wakeup < 0); // boucle jusqu'à pouvoir se réveiller
        semaphore->wakeup--; // consomme un réveil
    }
    unlock(&(semaphore->verrou));
}

// incrémente la valeur de la sémaphore, si elle est négative ou nulle,
// il y a des threads bloqués dans la file -> on débloque le premier
void mysem_post(mysem_t *semaphore) {

    lock(&(semaphore->verrou));
    semaphore->value++;

    if (semaphore->value <= 0) {
        semaphore->wakeup++; // un thread supplémentaire peut se réveiller
        cond_signal(semaphore->cond);
    }
    unlock(&(semaphore->verrou));
}

// destruction de la sémaphore
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

