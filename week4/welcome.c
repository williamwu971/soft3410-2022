//
// Created by Xiaoxiang Wu on 16/8/2022.
//

#include "../main.h"
#include <pthread.h>

struct thread_data {
    int tid;
};

void *work1(void *arg) {
    struct thread_data *data = (struct thread_data *) arg;
    printf("Hello from thread %d\n", data->tid);
    return NULL;
}

int main() {
    struct thread_data data[] = {{1},
                                 {2}};
    pthread_t threads[2];
    pthread_create(threads, NULL, work1, &data);
    pthread_create(threads + 1, NULL, work1, data + 1);
    printf("main thread has created threads\n");
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    printf("program finished\n");
    return 0;
}