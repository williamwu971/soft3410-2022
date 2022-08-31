//
// Created by xiaoxiang on 8/31/22.
//

#include "../main.h"

void multiply(const float *mata, size_t mata_width, size_t mata_height,
              const float *matb, size_t matb_width, size_t matb_height,
              float **result_mat, size_t *res_width, size_t *res_height,
              int fashion) {

    if (result_mat) {
        *res_width = matb_width;
        *res_height = mata_height;
        *result_mat = calloc(mata_height * matb_width, sizeof(float));

        puts("begin");

        T_declare_timer;
        T_start_timer;

        if (fashion) {
            for (size_t y = 0; y < mata_height; y++) {
                for (size_t k = 0; k < mata_width; k++) {
                    for (size_t x = 0; x < matb_width; x++) {
                        (*result_mat)[(y * matb_width) + x] +=
                                (mata[(y * mata_width) + k] *
                                 matb[(k * matb_width) + x]);
                    }
                }
            }
        } else {
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


        T_stop_timer("");
        fprintf(stderr, "%.2f", elapsed);
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

    pin(6);

    if (argc != 3) return 1;
    int size = atoi(argv[1]);
    int fashion = atoi(argv[2]);

    float *a = generate_mat(size, size);
    float *b = generate_mat(size, size);

    float *r;
    size_t r_width;
    size_t r_height;

    multiply(a, size, size,
             b, size, size,
             &r, &r_width, &r_height,
             fashion);

    return 0;
}