#ifndef NBODY_H
#define NBODY_H

#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define GCONST (6.67e-11)
#define BUF_LEN 512
#define NUM_OF_THREADS 7
#define SOLARMASS 999999999999
#define PLANETMASS 10000
#define LOCKED 1
#define UNLOCKED 0

void slave_wait(int *barrier) {

    __atomic_fetch_add(barrier, 1, __ATOMIC_SEQ_CST);

    int current;
    do {
        __atomic_load(barrier, &current, __ATOMIC_SEQ_CST);
    } while (current != 0);
}

void master_wait(int *barriers, int num_of_threads) {

    int go = 0;
    int latest_index = 0;

    // check status of every thread
    while (!go) {
        go = 1;
        for (int i = latest_index; i < num_of_threads; i++) {
            int current;
            __atomic_load(barriers + i, &current, __ATOMIC_SEQ_CST);

            if (!current) {
                latest_index = i;
                go = 0;
                break;
            }
        }
    }

    go = 0;

    // instruct them to go
    for (int i = 0; i < num_of_threads; i++) {
        __atomic_store(barriers + i, &go, __ATOMIC_SEQ_CST);
    }
}


struct body {
    double x;
    double y;
    double z;
    double velocity_x;
    double velocity_y;
    double velocity_z;
    double mass;
    int lock[2]; // padding
};

void slock(int *lock) {
    int expected = UNLOCKED;
    while (!__atomic_compare_exchange_n(lock, &expected, LOCKED, 1, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
        expected = UNLOCKED;
    }
}

void sunlock(int *lock) {

    int expected = LOCKED;
    __atomic_compare_exchange_n(lock, &expected, UNLOCKED, 1, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

typedef struct {

    double dt;
    struct body *bodies;
    int num_of_bodies;
    int start0;
    int start1;
    int end0;
    int end1;
    int *barrier_ptr;
    long num_of_iterations;

} step_package;

void *step_worker(void *arg) {

    step_package *sp = (step_package *) arg;

    for (long x = 0; x < sp->num_of_iterations; x++) {

        int i = sp->start0;
        while (i < sp->end1) {

            struct body *me = sp->bodies + i;
            double change_x = 0;
            double change_y = 0;
            double change_z = 0;

            for (int j = i + 1; j < sp->num_of_bodies; j++) {

                struct body *other = sp->bodies + j;


                double distance_squared =
                        pow(other->x - me->x, 2) +
                        pow(other->y - me->y, 2) +
                        pow(other->z - me->z, 2);

                double distance = sqrt(distance_squared);
                double magnitude = GCONST * me->mass * other->mass / distance_squared;

                // unit vector
                double d1 = (other->x - me->x) / distance;
                double d2 = (other->y - me->y) / distance;
                double d3 = (other->z - me->z) / distance;

                // acceleration vector
                double a1 = d1 * magnitude / me->mass;
                double a2 = d2 * magnitude / me->mass;
                double a3 = d3 * magnitude / me->mass;

                // reduce the need to hold lock
                change_x += a1 * sp->dt;
                change_y += a2 * sp->dt;
                change_z += a3 * sp->dt;

                // bidirection
                a1 = d1 * magnitude / other->mass;
                a2 = d2 * magnitude / other->mass;
                a3 = d3 * magnitude / other->mass;

                slock(other->lock);
                other->velocity_x -= a1 * sp->dt;
                other->velocity_y -= a2 * sp->dt;
                other->velocity_z -= a3 * sp->dt;
                sunlock(other->lock);

            }

            // apply a velocity change
            slock(me->lock);
            me->velocity_x += change_x;
            me->velocity_y += change_y;
            me->velocity_z += change_z;
            sunlock(me->lock);

            i++;
            if (i == sp->end0) {
                i = sp->start1;
            }
        }
        slave_wait(sp->barrier_ptr);

        i = sp->start0;
        while (i < sp->end1) {

            struct body *me = sp->bodies + i;

            // apply position change
            me->x += me->velocity_x * sp->dt;
            me->y += me->velocity_y * sp->dt;
            me->z += me->velocity_z * sp->dt;

            i++;
            if (i == sp->end0) {
                i = sp->start1;
            }
        }

        slave_wait(sp->barrier_ptr);
    }

    return NULL;
}

typedef struct {

    pthread_t tids[NUM_OF_THREADS];
    step_package sps[NUM_OF_THREADS];
    int thread_count;
    int *barriers;

} helper;

helper *start_program(double dt, struct body *bodies,
                      int num_of_bodies, long num_of_iterations) {

    helper *h = malloc(sizeof(helper));
    int per_thread = num_of_bodies / NUM_OF_THREADS;
    int remainder = num_of_bodies % NUM_OF_THREADS;
    int current_start = 0;
    int current_end = num_of_bodies;


    // create threads
    int i = 0;
    for (; i < NUM_OF_THREADS && current_start < current_end; i++) {

        int this_thread = per_thread + (remainder > 0 ? 1 : 0);
        if (remainder > 0) remainder--;

        // assign arguments
        h->sps[i] = (step_package) {.dt=dt,
                .bodies=bodies,
                .num_of_bodies=num_of_bodies,
                .start0=current_start,
                .end0=current_start + this_thread / 2 + this_thread % 2,
                .start1=current_end - this_thread / 2,
                .end1=current_end,
                .num_of_iterations=num_of_iterations};

//        printf("%d %d %d %d\n", h->sps[i].start0, h->sps[i].end0, h->sps[i].start1, h->sps[i].end1);
        current_start = h->sps[i].end0;
        current_end = h->sps[i].start1;
    }

    h->barriers = malloc(sizeof(int) * i);
    h->thread_count = i;

    for (int j = 0; j < i; j++) {
        h->sps[j].barrier_ptr = h->barriers + j;
        h->barriers[j] = 0; // implies incomplete
        pthread_create(h->tids + j, NULL, step_worker, h->sps + j);
    }


    return h;
}

void end_program(helper *h) {

    // join threads
    for (int j = 0; j < h->thread_count; j++) {
        pthread_cancel(h->tids[j]);
    }
    free(h->barriers);
    free(h);
}


void step(helper *h) {
    master_wait(h->barriers, h->thread_count);
    master_wait(h->barriers, h->thread_count);
}


double energy(struct body *bodies, int num_of_bodies) {

    double sum = 0;

    for (int i = 0; i < num_of_bodies; i++) {

        struct body me = bodies[i];
        double total_velocity =
                pow(me.velocity_x, 2) +
                pow(me.velocity_y, 2) +
                pow(me.velocity_z, 2);
        double self_energy = me.mass * total_velocity / 2;

        for (int j = i + 1; j < num_of_bodies; j++) {

            struct body other = bodies[j];

            double distance_squared = pow(other.x - me.x, 2) +
                                      pow(other.y - me.y, 2) +
                                      pow(other.z - me.z, 2);
            double distance = sqrt(distance_squared);
            self_energy -= me.mass * other.mass / distance;

        }

        sum += self_energy;

    }

    return sum;
}

//void debug_printer(struct body *bodies, int num_of_bodies) {
//
//    printf("num_of_bodies is %d to %p\n", num_of_bodies, bodies);
//
//    for (int i = 0; i < num_of_bodies; i++) {
//        struct body me = bodies[i];
//        printf("x: %f, y: %f, z: %f, vx: %f, vy: %f, vz: %f, mass: %f\n",
//               me.x, me.y, me.z, me.velocity_x, me.velocity_y, me.velocity_z, me.mass);
//    }
//}

#endif
