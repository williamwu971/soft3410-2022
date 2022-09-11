//
// Created by xiaoxiang on 9/11/22.
//

#include "../main.h"

int use_rwlock = 0;

struct dynamic_array {
    pthread_rwlock_t rwlock;
    pthread_mutex_t mutex;

    void **values;
    size_t capacity;
    size_t used;
};

struct dynamic_array *dynamic_array_new(size_t initial_capacity) {
    struct dynamic_array *array = malloc(sizeof(struct dynamic_array));

    if (use_rwlock) {
        pthread_rwlock_init(&array->rwlock, NULL);
        puts("using rwlock");
    } else {
        pthread_mutex_init(&array->mutex, NULL);
        puts("using mutex");
    }

    array->values = malloc(sizeof(void *) * initial_capacity);
    array->capacity = initial_capacity;
    array->used = 0;

    return array;
}

void dynamic_array_add(struct dynamic_array *ary, void *value) {

    if (use_rwlock) {
        pthread_rwlock_wrlock(&ary->rwlock);
    } else {
        pthread_mutex_lock(&ary->mutex);
    }

    if (ary->used == ary->capacity) {
        ary->capacity *= 2;
        ary->values = realloc(ary->values, ary->capacity * sizeof(void *));
    }
    ary->values[ary->used++] = value;

    if (use_rwlock) {
        pthread_rwlock_unlock(&ary->rwlock);
    } else {
        pthread_mutex_unlock(&ary->mutex);
    }
}

void *dynamic_array_get(struct dynamic_array *ary, size_t index) {
    if (use_rwlock) {
        pthread_rwlock_rdlock(&ary->rwlock);
    } else {
        pthread_mutex_lock(&ary->mutex);
    }

    void *value = NULL;
    if (index <= ary->used) {
        value = ary->values[index];
    }

    if (use_rwlock) {
        pthread_rwlock_unlock(&ary->rwlock);
    } else {
        pthread_mutex_unlock(&ary->mutex);
    }
    return value;
}

void *dynamic_array_remove(struct dynamic_array *ary, size_t index) {
    if (use_rwlock) {
        pthread_rwlock_wrlock(&ary->rwlock);
    } else {
        pthread_mutex_lock(&ary->mutex);
    }

    void *value = NULL;

    if (index < ary->used) {
        ary->used--;
        value = ary->values[index];
        memmove(ary->values + index, ary->values + index + 1, sizeof(void *) * ary->used - index);
    }

    if (use_rwlock) {
        pthread_rwlock_unlock(&ary->rwlock);
    } else {
        pthread_mutex_unlock(&ary->mutex);
    }
    return value;
}

void dynamic_array_destroy(struct dynamic_array *ary) {

    if (use_rwlock) {
        pthread_rwlock_destroy(&ary->rwlock);
    } else {
        pthread_mutex_destroy(&ary->mutex);
    }

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
        dynamic_array_add(array, malloc(1));
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

    use_rwlock = atoi(argv[1]);

    array = dynamic_array_new(64);

    for (int i = 0; i < 1000; i++) {
        dynamic_array_add(array, malloc(1));
    }

    declare_timer
    declare_parallel(N);

    write_workload = 10000000 * atoi(argv[2]) / 100;
    read_workload = 0;

    start_timer
    run_parallel(N, thread);stop_timer();


    read_workload = 10000000 - write_workload;
    write_workload = 0;

    start_timer
    run_parallel(N, thread);stop_timer();
}