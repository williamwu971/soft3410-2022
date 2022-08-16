//
// Created by Xiaoxiang Wu on 16/8/2022.
//

#include "../main.h"
#include <pthread.h>

#define SIZE 1000000000
#define T 10

uint64_t *numbers;

void *worker(void *arg) {

    uint64_t id = (uint64_t) arg;
    uint64_t start = SIZE / T * id;
    uint64_t end = SIZE / T * (id + 1);
    uint64_t sum = 0;


    for (; start < end; start++) {
        sum += numbers[start];
    }

    return (void *) sum;
}

int main() {


    numbers = malloc(sizeof(uint64_t) * SIZE);
    init_seed();

    for (uint64_t i = 0; i < SIZE; i++) {
        numbers[i] = lehmer64();
    }


    pthread_t threads[T];
    puts("begin");

    for (uint64_t i = 0; i < T; i++) {
        pthread_create(threads + i, NULL, worker, (void *) i);
    }

    uint64_t sum = 0;
    for (uint64_t i = 0; i < T; i++) {
        uint64_t local;
        pthread_join(threads[i], (void **) &local);
        sum += local;
    }

    printf("sum: %llu\n", sum);

}