//
// Created by xiaoxiang on 8/28/22.
//

#include "../main.h"

#include <time.h>
#include <stdio.h>

void *seq_cpy(size_t bytes, size_t iterations, void *allocation) {

    // todo: out-of-bounds handle

    void *dest = malloc(bytes * iterations);

    void *dest_tracker = dest;


    for (size_t i = 0; i < iterations; i++, dest_tracker += bytes, allocation += bytes) {
        memcpy(dest_tracker, allocation, bytes);
    }

    return dest;
}

void *rand_cpy(size_t bytes, size_t iterations, void *allocation, size_t *indexes) {

    void *dest = malloc(bytes * iterations);

    void *dest_tracker = dest;

    for (size_t i = 0; i < iterations; i++, dest_tracker += bytes) {
        memcpy(dest_tracker, allocation + indexes[i], bytes);
    }

    return dest;
}

void *generate_allocation(size_t nmemb, size_t size, size_t mod) {

    void *allocation = malloc(size * nmemb);

    for (size_t i = 0; i < nmemb; i++) {
        size_t *number = (size_t *) (allocation + i * size);

        number[0] = rand() % mod;
    }

    return allocation;
}

int main(int argc, char **argv) {

    srand(time(NULL));

    struct timespec start;
    struct timespec end;
    double elapsed;

    if (argc != 4) die("argc: %d", argc);
    int random = strcmp(argv[1], "rand") == 0;
    size_t bytes = atoi(argv[2]);
    size_t iterations = atoi(argv[3]);

    void *allocation = generate_allocation(iterations, bytes, 100);
    size_t *indexes = generate_allocation(iterations, sizeof(size_t), (iterations - 1) * bytes);

    char command[256];
    sprintf(command,
            "sudo /home/blepers/linux-huge/tools/perf/perf stat -e "
            "mem_load_retired.l1_hit,"
            "mem_load_retired.l1_miss,"
            "mem_load_retired.l2_hit,"
            "mem_load_retired.l2_miss,"
            "mem_load_retired.l3_hit,"
            "mem_load_retired.l3_miss"
            " -p %d &",
            getpid()
    );
    system(command);
    sleep(1);

    clock_gettime(CLOCK_MONOTONIC, &start);

    //Your function call here
    if (random) {
        rand_cpy(bytes, iterations, allocation, indexes);
    } else {
        seq_cpy(bytes, iterations, allocation);
    }


    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (double) (end.tv_sec - start.tv_sec);
    elapsed += (double) (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    system("sudo killall -s INT -w perf");

    //output of time measured in seconds
    fprintf(stderr, "timed %s %.2fs\n", argv[1], elapsed);

}