//
// Created by xiaoxiang on 8/12/22.
//

#include "../main.h"

#define N 1000

void regular(int *a, int *b, int *c) {
    //Regular loop
    for (int i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
    }
}

void unrolled(int *a, int *b, int *c) {
    //Unrolled
    for (int i = 0; i < N; i += 4) {
        c[i] = a[i] + b[i];
        c[i + 1] = a[i + 1] + b[i + 1];
        c[i + 2] = a[i + 2] + b[i + 2];
        c[i + 3] = a[i + 3] + b[i + 3];
    }
}


int *generate(int init) {
    if (!init) return calloc(N, sizeof(int));

    srand(time(NULL));
    int *numbers = malloc(N * sizeof(int));

    for (int i = 0; i < N; i++) {
        numbers[i] = rand() % 100;
    }

    return numbers;
}

int main() {

    int *a = generate(1);
    int *b = generate(1);
    int *c = generate(0);

    declare_timer
    start_timer
    {
        regular(a, b, c);
    }stop_timer("reguler");

    a = generate(1);
    b = generate(1);
    c = generate(0);

    start_timer
    {
        unrolled(a, b, c);
    }stop_timer("unrolled");

    return 0;
}