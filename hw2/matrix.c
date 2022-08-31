//
// Created by xiaoxiang on 8/31/22.
//

#include "../main.h"

void multiply(const float *mata, size_t mata_width, size_t mata_height,
              const float *matb, size_t matb_width, size_t matb_height,
              float **result_mat, size_t *res_width, size_t *res_height) {

    if (result_mat) {
        *res_width = matb_width;
        *res_height = mata_height;
        *result_mat = calloc(mata_height * matb_width, sizeof(float));

        puts("begin");

        T_declare_timer;
        T_start_timer;

        for (size_t y = 0; y < mata_height; y++) {

                for (size_t k = 0; k < mata_width; k++) {

                    for (size_t x = 0; x < matb_width; x++) {
                    (*result_mat)[(y * matb_width) + x] +=
                            (mata[(y * mata_width) + k] *
                             matb[(k * matb_width) + x]);
                }
            }
        }

        T_stop_timer("");
    }
}

float *generate_mat(size_t mata_width, size_t mata_height) {

    float *mat = malloc(mata_width * mata_height * sizeof(float));

    for (size_t i = 0; i < mata_height; i++) {
        for (size_t j = 0; j < mata_width; j++) {
            mat[i * mata_width + j] = i * mata_width + j;
        }
    }

    return mat;
}

int main(int argc, char **argv) {

    if (argc != 2) return 1;
    int size = atoi(argv[1]);

    float *a = generate_mat(size, size);
    float *b = generate_mat(size, size);

    float *r;
    size_t r_width;
    size_t r_height;

    multiply(a, size, size,
             b, size, size,
             &r, &r_width, &r_height);

    return 0;
}