//
// Created by xiaoxiang on 8/12/22.
//

#include "../main.h"

#define WIDTH 1024
#define IDX(x, y) ((y)*WIDTH+(x))

void multiply(const float *mata, size_t mata_width, size_t mata_height,
              const float *matb, size_t matb_width, size_t matb_height,
              float **result_mat, size_t *res_width, size_t *res_height) {
    if (result_mat) {
        *res_width = matb_width;
        *res_height = mata_height;
        *result_mat = calloc(mata_height * matb_width, sizeof(float));
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

float *hadamard(void) {
    assert(((WIDTH - 1) & WIDTH) == 0);
    size_t w = WIDTH;
    size_t quad_size = 1;
    float *result = malloc(WIDTH * WIDTH * sizeof(float));
    result[0] = 1;
    while ((w >>= 1) != 0) {
        for (size_t y = 0; y < quad_size; ++y) {
            for (size_t x = 0; x < quad_size; ++x) {
                const float v = result[IDX(x, y)];
                result[IDX(x + quad_size, y)] = v;
                result[IDX(x, y + quad_size)] = v;
                result[IDX(x + quad_size, y + quad_size)] = -v;
            }
        }
        quad_size *= 2;
    }
    return result;
}

// Displays a matrix.
void display(const float *matrix) {
    for (size_t y = 0; y < WIDTH; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            printf("%6.2f ", matrix[IDX(x, y)]);
        }
        printf("\n");
    }
}

int main(void) {
// Construct the matrices
    float *a = hadamard();
    float *b = hadamard();

//    display(a);
//    display(b);


// Compute the result
    size_t rwidth = 0;
    size_t rheight = 0;
    float *result = NULL;
    multiply(a, WIDTH, WIDTH,
             b, WIDTH, WIDTH,
             &result, &rwidth, &rheight);

//    display(result);
// Verify the result
    for (size_t y = 0; y < WIDTH; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            assert(x == y ? result[IDX(x, y)] == WIDTH : !result[IDX(x, y)]);
        }
    }
    puts("done");
    free(a);
    free(b);
    free(result);
    return 0;
}