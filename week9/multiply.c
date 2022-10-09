//
// Created by xiaoxiang on 10/9/22.
//

#include "../main.h"

void multiply(const float *mata, size_t mata_width, size_t mata_height,
              const float *matb, size_t matb_width, size_t matb_height,
              float **result_mat, size_t *res_width, size_t *res_height) {
    if (result_mat) {
        *res_width = matb_width;
        *res_height = mata_height;
        *result_mat = calloc(mata_height * matb_width, sizeof(float));

#pragma omp parallel for
        for (size_t y = 0; y < mata_height; y++) {
            for (size_t x = 0; x < matb_width; x++) {
                for (size_t k = 0; k < mata_width; k++) {
                    (*result_mat)[(y * matb_width) + x] +=
                            (mata[(y * mata_width) + k] *
                             matb[(k * matb_width) + x]);
                }
            }
        }
    }
}

int main() {

    size_t N = 2000;

    float *a = malloc(sizeof(float) * N * N);
    memset(a, 7, sizeof(float) * N * N);

    float *b = malloc(sizeof(float) * N * N);
    memset(b, 7, sizeof(float) * N * N);

    float *result;
    size_t rw, rh;
    multiply(a, N, N, b, N, N, &result, &rw, &rh);

    assert(result != NULL);
    return 0;

}