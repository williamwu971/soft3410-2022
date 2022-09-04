//
// Created by xiaoxiang on 9/4/22.
//

#include "../main.h"

#define THINKERS 4

struct args {
    pthread_mutex_t *locks;
    sem_t *sem;
    unsigned int id;
};

void *dine(void *arg) {

    struct args a = *((struct args *) arg);
    const unsigned id = a.id;

//    pthread_mutex_t *chopstick0;
//    pthread_mutex_t *chopstick1;
//
//    chopstick0 = a.locks + id;
//    chopstick1 = a.locks + (id + 1) % THINKERS;

//    if (id != THINKERS - 1) {
//        chopstick0 = a.locks + id;
//        chopstick1 = a.locks + id + 1;
//    } else {
//        chopstick0 = a.locks;
//        chopstick1 = a.locks + id;
//    }

    for (int i = 0; i < 1000; i++) {

        // TODO: Acquire two chopsticks first
        // the ith philosopher can only reach
        // the ith and (i + 1)th chopstick

//        pthread_mutex_lock(chopstick0);
//        pthread_mutex_lock(chopstick1);

        sem_wait(a.sem);

        printf("Philosopher %u is eating\n", id);

        sem_post(a.sem);

//        pthread_mutex_unlock(chopstick0);
//        pthread_mutex_unlock(chopstick1);
    }
    return NULL;
}

int main(void) {
    struct args dine_args[THINKERS];
    pthread_t thinkers[THINKERS];
    pthread_mutex_t chopsticks[THINKERS];

    sem_t sem;
    sem_init(&sem, 0, THINKERS / 2);

    // create the chopsticks
    for (size_t i = 0; i < THINKERS; i++) {
        if (pthread_mutex_init(chopsticks + i, NULL) != 0) {
            perror("unable to initialize mutex");
            return 1;
        }
    }
    // launch threads
    for (size_t i = 0; i < THINKERS; i++) {
        dine_args[i].id = i;
        dine_args[i].locks = chopsticks;
        dine_args[i].sem = &sem;
        if (pthread_create(thinkers + i, NULL, dine, dine_args + i) != 0) {
            perror("unable to create thread");
            return 1;
        }
    }
    // wait for threads to finish
    for (size_t i = 0; i < THINKERS; i++) {
        if (pthread_join(thinkers[i], NULL) != 0) {
            perror("unable to join thread");
            return 1;
        }
    }

    // remove the chopsticks
    for (size_t i = 0; i < THINKERS; i++) {
        if (pthread_mutex_destroy(chopsticks + i) != 0) {
            perror("unable to destroy mutex");
            return 1;
        }
    }
    return 0;
}