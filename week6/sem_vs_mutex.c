//
// Created by Xiaoxiang Wu on 5/9/2022.
//

#include "../main.h"

sem_t *sem;
pthread_mutex_t *mutex;

int iter;

void *func() {

    for (int i = 0; i < iter; i++) {
//        sem_wait(sem);
//        sem_post(sem);

        pthread_mutex_lock(mutex);
        pthread_mutex_unlock(mutex);
    }

    return NULL;
}

int main(int argc, char **argv) {


    int num_thread = atoi(argv[2]);
    iter = atoi(argv[1]) / num_thread;

    sem_t s;
    pthread_mutex_t m;

    sem_init(&s, 0, 1);
    pthread_mutex_init(&m, NULL);

    sem = &s;
    mutex = &m;

    declare_parallel(num_thread);
    declare_timer
    start_timer

    run_parallel(num_thread, func);


    stop_timer();
}