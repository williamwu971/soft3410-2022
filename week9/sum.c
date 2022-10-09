//
// Created by xiaoxiang on 10/9/22.
//

#include "../main.h"

int main() {
//data here

    size_t end = 1000000000;

    int *data = malloc(sizeof(int) * end);
    memset(data, 7, sizeof(int) * end);

    int64_t sum = 0;


    //todo: is this correct?
#pragma omp parallel for
    for (size_t i = 0; i < end; i++) {
        sum += data[i];
    }
    printf("%ld\n", sum);
    return 0;
}