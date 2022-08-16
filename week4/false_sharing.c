//
// Created by Xiaoxiang Wu on 16/8/2022.
//

#include "../main.h"
#include <pthread.h>

#define T 8

struct thread_data {
    int *result;
    int result_index;
    int *numbers;
    int numbers_len;
};

void *worker(void *data) {
    struct thread_data *d = (struct thread_data *) data;
    for (int i = 0; i < d->numbers_len; i++) {
        d->result[d->result_index] += d->numbers[i];
    }
    return NULL;
}

int main() {

    init_seed();
    int numbers_len = 800000000;
    int numbers_len_per_t = numbers_len / T;
    int *numbers = malloc(numbers_len * sizeof(int));
    for (int i = 0; i < numbers_len; i++) {
//        numbers[i] = (int) lehmer64();
        numbers[i] = i;
    }

    pthread_t threads[T];
    struct thread_data datas[T];
    int result[T];

    for (int i = 0; i < T; i++) {
//        datas[i].result = result + i;
        posix_memalign((void **) &datas[i].result, 64, sizeof(int));
        datas[i].result[0] = 0;
        datas[i].result_index = 0;
        datas[i].numbers = numbers;
        datas[i].numbers_len = numbers_len_per_t;

        numbers += numbers_len_per_t;
    }

    declare_timer

    start_timer
    {
        for (int i = 0; i < T; i++) {
            pthread_create(threads + i, NULL, worker, datas + i);
        }

        int sum = 0;
        for (int i = 0; i < T; i++) {
            pthread_join(threads[i], NULL);
//        sum += result[i];
            sum += datas[i].result[0];
        }

        printf("sum: %d\n", sum);
    }stop_timer();


    return 0;
}