//
// Created by xiaoxiang on 10/9/22.
//

#include "../main.h"

struct box {
    int x;
    int width;
    int y;
    int height;
};

int intersect(struct box box1, struct box box2) {
    return (box1.x < (box2.x + box2.width)) &&
           ((box1.x + box1.width) > box2.x) &&
           (box1.y < (box2.y + box2.height)) &&
           ((box1.y + box1.height) > box2.y);
}

int main() {

    size_t num_box = 100000;
    struct box *boxes = malloc(sizeof(struct box) * num_box);

    for (size_t i = 0; i < num_box; i++) {
        boxes[i] = (struct box) {.x=rand(), .y=rand(), .height=100, .width=100};
    }

    for (uint64_t i = 0; i < 10; i++) {

        // todo: update box position
        // todo: barrier
        // todo: workload balance

        int inter = 0;

#pragma omp parallel for
        for (size_t j = 0; j < num_box; j++) {
            for (size_t x = j + 1; x < num_box; x++) {
                if (intersect(boxes[i], boxes[j])) {
                    inter++;
                }
            }
        }

        printf("inter: %d\n", inter);
    }

}
