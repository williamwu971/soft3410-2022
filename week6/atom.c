//
// Created by xiaoxiang on 9/4/22.
//

#include "../main.h"

atomic_int counter = 0;

#define ITER 100000000
int T;

void *increment() {

    uint64_t iter = ITER / T;

    for (uint64_t i = 0; i < iter; i++) {
        atomic_fetch_add(&counter, 1);
    }
}


int main(int argc, char **argv) {

    if (argc != 2) return 1;
    T = atoi(argv[1]);

    declare_parallel(T);

    declare_timer
    start_timer

    run_parallel(T, increment);

    stop_timer();
    fprintf(stderr, "%lu", elapsed/1000);
}
