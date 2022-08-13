//
// Created by xiaoxiang on 8/12/22.
//

#include "../main.h"
#include <math.h>

void resize_nn(int *pixels_in, size_t in_width, size_t in_height,
               int *pixels_out, size_t out_width, size_t out_height) {
    if (pixels_in
        && pixels_out && out_width && out_height
        && in_width && in_height) {
        double x_ratio = in_width / (double) out_width;
        double y_ratio = in_height / (double) out_height;
        for (size_t y = 0; y < out_height; y++) {
            for (size_t x = 0; x < out_width; x++) {
                double adj_x = floor(x * x_ratio);
                double adj_y = floor(y * y_ratio);
                pixels_out[(y * out_width) + x] =
                        pixels_in[(size_t)
                                ((adj_y * in_width) + adj_x)];
            }
        }
    }
}

int *generate(size_t in_width, size_t in_height, int fill) {

    if (!fill) return calloc(in_width * in_height, sizeof(int));

    int *out = malloc(sizeof(int) * in_width * in_height);
    srand(time(NULL));

    for (size_t y = 0; y < in_height; y++) {
        for (size_t x = 0; x < in_width; x++) {
            out[y * in_width + x] = rand() % 256;
        }
    }

    return out;
}

int main() {

    int *in_pic = generate(640, 360, 1);
    int *out_pic = generate(7680, 4320, 0);

    resize_nn(in_pic, 640, 360, out_pic, 7680, 4320);

    return 0;

}