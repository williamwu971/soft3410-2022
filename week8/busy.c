//
// Created by xiaoxiang on 9/18/22.
//

#include "busy.h"

struct sync_barrier *sync_barrier_new(uint16_t threshold) {
    struct sync_barrier *barrier = calloc(1, sizeof(struct sync_barrier));

    barrier->threshold = threshold;
    atomic_store(&barrier->counter, 0);

    return barrier;
}

void sync_barrier_wait(struct sync_barrier *barrier) {

    atomic_uint old = atomic_load(&barrier->release_count);

    atomic_uint counter = atomic_fetch_add(&barrier->counter, 1);
    if (counter == barrier->threshold) {
        atomic_store(&barrier->counter, 0);
        atomic_fetch_add(&barrier->release_count, 1);
    } else {
        while (1) {
            if (atomic_load(&barrier->release_count) != old) {
                break;
            }
        }
    }
}

void sync_barrier_destroy(struct sync_barrier *barrier) {
    memset(barrier, 1, sizeof(struct sync_barrier));
    free(barrier);

    // todo: what else?
}
