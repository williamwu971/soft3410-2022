//
// Created by Xiaoxiang Wu on 5/9/2022.
//

#include "../main.h"

sem_t *sem;
pthread_mutex_t *mutex;

void *func() {

    for (int i = 0; i < 10000; i++) {
        sem_wait(sem);
        sem_post(sem);
    }

    return NULL;
}

int main(int argc, char **argv) {


    sem_t s;
    pthread_mutex_t m;

    sem_init(&s, 0, 1);
    pthread_mutex_init(&m, NULL);

    sem = &s;
    mutex = &m;

    declare_parallel(4);
    declare_timer
    start_timer

    run_parallel(4, func);


    stop_timer();
}