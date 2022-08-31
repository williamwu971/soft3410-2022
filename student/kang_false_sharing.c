//
// Created by xiaoxiang on 8/31/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N (700000000)
#define ARGC_SZ (2)
//#define CACHE_LINE_ALIGN
#define CACHE_LINE_B (64)
#define S_TO_MS (1000)
#define NS_TO_MS (1000000)
// #define DEBUG

struct thread_data {
    int *result;
    int result_index;
    int *numbers;
    int numbers_len;
};

int *fill_int_array(int *arr, size_t sz) {
    for (int i = 0; i < sz; i++) {
        // arr[i] = rand();
        arr[i] = i;
    }
    return arr;
}

void *worker(void *data) {
    struct thread_data *d = (struct thread_data *) data;

    // printf("%p\n",(d->result + (d->result_index * (CACHE_LINE_B / sizeof(int)))));
    for (int i = 0; i < d->numbers_len; i++) {
# ifdef CACHE_LINE_ALIGN
        *(d->result + (d->result_index * (CACHE_LINE_B / sizeof(int)))) += d->numbers[i];
# else
        d->result[d->result_index] += d->numbers[i];
#endif
    }
    return NULL;
}

int arr_sum(int *arr, size_t n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

int main(int argc, char **argv) {
    if (argc < ARGC_SZ) {
        printf("Usage: parallel_sum <thread_number>\n");
        return 1;
    }

    int n_thread = atoi(argv[1]);
    int *arr = malloc(sizeof(int) * N);
    struct timespec start_time;
    struct timespec end_time;
    fill_int_array(arr, N);
    if (n_thread > 1) {

#ifdef CACHE_LINE_ALIGN
        int *result = calloc(CACHE_LINE_B, n_thread);
# else
        int *result = calloc(sizeof(int), n_thread);
#endif
        struct thread_data *thread_data = malloc(sizeof(struct thread_data) * n_thread);
        pthread_t threads[n_thread];

        size_t chunk_sz = (N / n_thread) + 1;
        size_t last_t_end = 0;
        for (int i = 0; i < n_thread; i++) {
            size_t start = last_t_end;
            struct thread_data *data = &thread_data[i];
            data->result = result;
            data->result_index = i;
            data->numbers = &arr[last_t_end];
            if (i == n_thread - 1 && start + chunk_sz > N) {
                data->numbers_len = N - start;
                last_t_end += data->numbers_len;
            } else {
                data->numbers_len = chunk_sz;
                last_t_end += chunk_sz;
            }
            clock_gettime(CLOCK_MONOTONIC, &start_time);
            pthread_create(&threads[i], NULL, worker, &thread_data[i]);
#ifdef DEBUG
            printf("Created thread %d, chunk_sz %d\n", i, data->numbers_len);
#endif
        }
        for (int i = 0; i < n_thread; i++) {
            pthread_join(threads[i], NULL);
        }

        int sum = 0;
        for (int i = 0; i < n_thread; i++) {
#ifdef CACHE_LINE_ALIGN
            sum += *(result + (i * (CACHE_LINE_B / sizeof(int))));
#else
            sum += result[i];
#endif
        }
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        free(thread_data);
        free(result);
#ifdef DEBUG
        printf("Parallel sum: %d\n", sum);
#endif
    } else {
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        int sum = arr_sum(arr, N);
        clock_gettime(CLOCK_MONOTONIC, &end_time);
#ifdef DEBUG
        printf("Serial sum: %d\n", sum);
#endif
    }
    free(arr);
    double ms = ((end_time.tv_sec - start_time.tv_sec) * S_TO_MS) +
                (((double) (end_time.tv_nsec - start_time.tv_nsec)) / NS_TO_MS);
    fprintf(stderr, "%lf", ms);
    return 0;
}