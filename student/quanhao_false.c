//
// Created by xiaoxiang on 8/23/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define TN (16)
#define RN (1000000)
#define PADDING (128)

struct thread_data {
    int *result;
    int result_index;
    int *numbers;
    int numbers_len;
};

void *worker(void *data) {
    struct thread_data *d = (struct thread_data *) data;

    for (int i = 0; i < d->numbers_len; i += 1) {
        d->result[d->result_index] += d->numbers[i];
    }
    return NULL;
}

int main() {

    pthread_t threads[TN];

    struct thread_data s;

    int *num = malloc((sizeof(int)) * RN);
    for (int i = 0; i < RN; i++) {
        num[i] = 1;
    }
    s.numbers = num;
    s.numbers_len = RN;

    int *res = malloc(sizeof(int) * TN);

    for (int i = 0; i < TN; i++) {
        res[i] = 0;
    }

    s.result = res;
    for (int i = 0; i < TN; i++) {
        s.result_index = i;
        pthread_create(threads + i, NULL, worker, &s);
    }

    for (int i = 0; i < TN; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < TN; i++) {
        printf("%d\n", res[i]);
    }


    free(res);
    free(num);

    return 0;
}