#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../main.h"

//#define TN (2)
#define RN (1000000000)
#define PADDING (128)

struct thread_data {
    long *results;
    long n_iter;
    int res_index;

};

void *worker(void *data) {
    struct thread_data *d = (struct thread_data *) data;

    for (int i = 0; i < d->n_iter; i++) {
        d->results[d->res_index] += 1;
    }
    return NULL;
}

int main(int argc, char **argv) {

    int TN = atoi(argv[1]);
    pthread_t threads[TN];

    struct thread_data s[TN];
    long *results = malloc(sizeof(long) * TN * PADDING);
    long total = 0;

    declare_timer
    start_timer

    for (int i = 0; i < TN; i++) {
        s[i].results = results;
        s[i].res_index = i * PADDING;
        s[i].n_iter = RN / TN;
        pthread_create(threads + i, NULL, worker, s + i);
    }

    for (int i = 0; i < TN; i++) {
        pthread_join(threads[i], NULL);
        total += results[s[i].res_index];
    }

    stop_timer();

    printf("TOTAL: %ld\n", total);
    fprintf(stderr, "%lu", elapsed / 1000);

    return 0;
}