//
// Created by Xiaoxiang Wu on 6/8/2022.
//

#ifndef SOFT3410_2022_SORTING_H
#define SOFT3410_2022_SORTING_H

#include "../main.h"
#include <math.h>

struct location {
    int x;
    int y;
    char *name;
};

void sort(int origin_x, int origin_y, struct location *locs,
          unsigned len);

#endif //SOFT3410_2022_SORTING_H
