//
// Created by xiaoxiang on 9/11/22.
//

#include "../main.h"

struct dynamic_array {
    pthread_rwlock_t rwlock;
    pthread_mutex_t mutex;

    int use_rwlock;

    void **values;
    size_t capacity;
    size_t used;
};

void dynamic_array_lock_init(struct dynamic_array *array) {
    if (array->use_rwlock) {
        pthread_rwlock_init(&array->rwlock, NULL);
        puts("using rwlock");
    } else {
        pthread_mutex_init(&array->mutex, NULL);
        puts("using mutex");
    }
}

void dynamic_array_lock_destroy(struct dynamic_array *ary) {
    if (ary->use_rwlock) {
        pthread_rwlock_destroy(&ary->rwlock);
    } else {
        pthread_mutex_destroy(&ary->mutex);
    }
}

void dynamic_array_write_lock(struct dynamic_array *ary) {
    int ret;
    if (ary->use_rwlock) {
        ret = pthread_rwlock_wrlock(&ary->rwlock);
    } else {
        ret = pthread_mutex_lock(&ary->mutex);
    }

    if (ret) die("dynamic_array_write_lock");
}

void dynamic_array_read_lock(struct dynamic_array *ary) {
    int ret;
    if (ary->use_rwlock) {
        ret = pthread_rwlock_rdlock(&ary->rwlock);
    } else {
        ret = pthread_mutex_lock(&ary->mutex);
    }
    if (ret)die("dynamic_array_read_lock");
}

void dynamic_array_unlock(struct dynamic_array *ary) {
    int ret;
    if (ary->use_rwlock) {
        ret = pthread_rwlock_unlock(&ary->rwlock);
    } else {
        ret = pthread_mutex_unlock(&ary->mutex);
    }
    if (ret)die("dynamic_array_unlock");
}

struct dynamic_array *dynamic_array_new(size_t initial_capacity, int use_rwlock) {
    struct dynamic_array *array = calloc(1, sizeof(struct dynamic_array));


    array->values = malloc(sizeof(void *) * initial_capacity);
    array->capacity = initial_capacity;
    array->used = 0;
    array->use_rwlock = use_rwlock;

    dynamic_array_lock_init(array);

    return array;
}

void dynamic_array_add(struct dynamic_array *ary, void *value) {

    dynamic_array_write_lock(ary);

    if (ary->used == ary->capacity) {
        ary->capacity *= 2;
        ary->values = realloc(ary->values, ary->capacity * sizeof(void *));
    }
    ary->values[ary->used++] = value;

    dynamic_array_unlock(ary);
}

void *dynamic_array_get(struct dynamic_array *ary, size_t index) {

    dynamic_array_read_lock(ary);

    void *value = NULL;
    if (index <= ary->used) {
        value = ary->values[index];
    }

    dynamic_array_unlock(ary);
    return value;
}

void *dynamic_array_remove(struct dynamic_array *ary, size_t index) {
    dynamic_array_write_lock(ary);

    void *value = NULL;

    if (index < ary->used) {
        ary->used--;
        value = ary->values[index];
        memmove(ary->values + index, ary->values + index + 1, sizeof(void *) * ary->used - index);
    }

    dynamic_array_unlock(ary);
    return value;
}

void dynamic_array_destroy(struct dynamic_array *ary) {

    dynamic_array_lock_destroy(ary);

    free(ary->values);
    free(ary);
}

uint64_t write_workload;
uint64_t read_workload;
struct dynamic_array *array;

void *thread(void *arg) {

    (void) arg;

    uint64_t sum = 0;

    for (uint64_t i = 0; i < write_workload; i++) {

        // write workload
        uint8_t *num = malloc(1);
        *num = 3;
        dynamic_array_add(array, num);
    }

    for (uint64_t i = 0; i < read_workload; i++) {
        // read workload
        uint8_t *num = dynamic_array_get(array, i);
        if (num != NULL) sum += *num;
    }

    return (void *) sum;

}

#define N 7

int main(int argc, char **argv) {

    if (argc != 3) return 1;

    int use_rwlock = atoi(argv[1]);
    double throughput;

    array = dynamic_array_new(64, use_rwlock);

    for (int i = 0; i < 1000; i++) {
        dynamic_array_add(array, malloc(1));
    }

    declare_timer
    declare_parallel(N);

    write_workload = 10000000 * atoi(argv[2]) / 100;
    read_workload = 0;
    printf("write: %lu read: %lu\n", write_workload, read_workload);

    start_timer
    run_parallel(N, thread);stop_timer();
    fprintf(stderr, "%f", bandwith(write_workload, elapsed));


    read_workload = 10000000 - write_workload;
    write_workload = 0;
    printf("write: %lu read: %lu\n", write_workload, read_workload);
    fprintf(stderr, "%f", bandwith(read_workload, elapsed));

    start_timer
    run_parallel(N, thread);stop_timer();
}