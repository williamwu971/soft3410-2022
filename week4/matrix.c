//
// Created by xiaoxiang on 8/16/22.
//

//
// Created by xiaoxiang on 8/15/22.
//

#include "../main.h"
#include <omp.h>


#define SIZE 3072
#define IDX(y, x) (((y)*SIZE)+(x))

int main() {

    omp_set_num_threads(8);

    u_int64_t *A = malloc(sizeof(u_int64_t) * SIZE * SIZE);
    u_int64_t *B = malloc(sizeof(u_int64_t) * SIZE * SIZE);
    u_int64_t *C = malloc(sizeof(u_int64_t) * SIZE * SIZE);

    srand(time(NULL));

#pragma omp parallel for schedule(static, 1)
    for (u_int64_t i = 0; i < SIZE; i++) {
        for (u_int64_t j = 0; j < SIZE; j++) {
            A[IDX(i, j)] = rand();
            B[IDX(i, j)] = rand();
            C[IDX(i, j)] = 0;
        }
    }

    puts("begin");

    declare_timer
    start_timer
    {

#pragma omp parallel for schedule(static, 1)
        for (u_int64_t i = 0; i < SIZE; i++) {
            for (u_int64_t k = 0; k < SIZE; k++) {
                for (u_int64_t j = 0; j < SIZE; j++) {


                    C[IDX(i, j)] += A[IDX(i, k)] * B[IDX(k, j)];
                }
            }
        }

    }stop_timer();


}