//
// Created by xiaoxiang on 9/18/22.
//

#ifndef SOFT3410_2022_BUSY_H
#define SOFT3410_2022_BUSY_H

#include "../main.h"

struct sync_barrier {
    uint16_t threshold;
    atomic_uint counter;
    atomic_uint release_count;
};

struct sync_barrier *sync_barrier_new(uint16_t);

void sync_barrier_wait(struct sync_barrier *);

void sync_barrier_destroy(struct sync_barrier *);

#endif //SOFT3410_2022_BUSY_H
