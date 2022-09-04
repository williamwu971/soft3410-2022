//
// Created by xiaoxiang on 9/4/22.
//

#include "../main.h"
#include "spin.h"

struct thread_data {
    int value;
    struct tas *t;
};

void *work(void *arg) {
    struct thread_data *data = (struct thread_data *) arg;
    for (int i = 0; i < 1000000; i++) {
        tas_lock(data->t);
        data->value += 1;
        tas_unlock(data->t);
    }
    return NULL;
}

int main() {

    struct tas t;
    tas_init(&t);

    struct thread_data data = {
            0, &t
    };
    pthread_t threads[2];
    pthread_create(threads, NULL, work, &data);
    pthread_create(threads + 1, NULL, work, &data);
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("%d\n", data.value);
    tas_destroy(&t);
    return 0;
}