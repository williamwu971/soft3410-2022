//
// Created by xiaoxiang on 9/4/22.
//

#include "spin.h"

void tas_init(struct tas *t) {
    atomic_store(&t->counter, 0);
}

int tas_lock(struct tas *t) {
    int expected = 0;
    while (!atomic_compare_exchange_strong(&t->counter, &expected, 1)) {
        if (expected != 1)die("lock invalid");
        expected = 0;
    }
}

int tas_unlock(struct tas *t) {

    int expected = 1;
    if (!atomic_compare_exchange_strong(&t->counter, &expected, 0)) {
        die("lock invalid");
    }
}

void tas_destroy(struct tas *t) {
    atomic_store(&t->counter, 2);
}
