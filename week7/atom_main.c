//
// Created by xiaoxiang on 9/11/22.
//

#include "atom_stack.h"

struct stack *s;

void *thread(void *arg) {
    for (int i = 0; i < 10000000; i++) {
        uint64_t *buf = malloc(sizeof(uint64_t));
        buf[0] = i;
        stack_push(s, buf);
    }

    uint64_t sum = 0;
    for (int i = 0; i < 10000000; i++) {
        sum += ((uint64_t *) stack_pop(s))[0];
    }

    return (void *) sum;
}

#define N 4

int main() {


    s = stack_new();

    declare_timer
    declare_parallel(N);

    start_timer
    run_parallel(N, thread);
    stop_timer();


    uint64_t sum = 0;
    for (int i = 0; i < N; i++) {
        sum += (uint64_t) returns[i];
    }
    printf("sum %lu\n", sum);
}