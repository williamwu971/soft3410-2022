//
// Created by Xiaoxiang Wu on 6/8/2022.
//

#include "sorting.h"

int dis(int x0, int y0, int x1, int y1) {

    return sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
}

void sort(int origin_x, int origin_y, struct location *locs, unsigned len) {


    // bubble sort

    int sorted = 0;

    while (!sorted) {

        sorted = 1;

        for (unsigned i = 0; i < len - 1; i++) {

            int dis0 = dis(origin_x, origin_y, locs[i].x, locs[i].y);
            int dis1 = dis(origin_x, origin_y, locs[i + 1].x, locs[i + 1].y);

            if (dis0 > dis1) {
                sorted = 0;

                struct location tmp = locs[i];

                locs[i] = locs[i + 1];
                locs[i + 1] = tmp;
            }
        }
    }

}


int main() {

    struct location locations[10];
    srand(time(NULL));

    for (int i = 0; i < 10; i++) {
        locations[i].x = rand() % 10;
        locations[i].y = rand() % 10;
    }

    sort(0, 0, locations, 10);

    for (int i = 0; i < 10; i++) {
        printf("point (%d,%d) dis %d\n", locations[i].x, locations[i].y,
               dis(0, 0, locations[i].x, locations[i].y));
    }

    return 0;

}