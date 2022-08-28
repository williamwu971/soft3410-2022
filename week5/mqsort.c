//
// Created by xiaoxiang on 8/28/22.
//

#include "../main.h"

void print_array(uint64_t *array, uint64_t size) {
    for (int i = 0; i < size; i++) {
        printf("%lu ", array[i]);
    }

    printf("\n");
}

void check_array(uint64_t *array, uint64_t size) {
    for (int i = 0; i < size - 1; i++) {
        if (array[i] > array[i + 1]) {
            die("%lu vs %lu", array[i], array[i + 1]);
        }
    }

    printf("check pass\n");
}

uint64_t *generate_allocation(size_t size, size_t mod) {

    uint64_t *allocation = malloc(size * sizeof(uint64_t));

    for (size_t i = 0; i < size; i++) {

        allocation[i] = rand() % mod;
    }

    return allocation;
}

void merge_sort(uint64_t *array, uint64_t size) {


    // todo: assuming size is divisible by 2

    uint64_t unit = 2;
    uint64_t *tmp = malloc(size * sizeof(uint64_t));

    while (unit <= size) {

        uint64_t half = unit / 2;

        for (uint64_t unit_idx = 0; unit_idx < size / unit; unit_idx++) {


            uint64_t *left = array + unit_idx * unit;
            uint64_t *right = array + unit_idx * unit + half;
            uint64_t left_idx = 0;
            uint64_t right_idx = 0;


            uint64_t *start = left;

            for (uint64_t i = 0; i < unit; i++) {
                if (left_idx == half) {
                    tmp[i] = right[right_idx];
                    right_idx++;
                    continue;
                } else if (right_idx == half) {
                    tmp[i] = left[left_idx];
                    left_idx++;
                    continue;
                }

                if (left[left_idx] < right[right_idx]) {
                    tmp[i] = left[left_idx];
                    left_idx++;
                } else {
                    tmp[i] = right[right_idx];
                    right_idx++;
                }

            }

            memcpy(start, tmp, sizeof(uint64_t) * unit);

        }
        unit *= 2;

//        puts("===");
    }
}

int compar(const void *a, const void *b) {
    return ((uint64_t *) a)[0] > ((uint64_t *) b)[0];
}


void quick_sort_correct(uint64_t *array, uint64_t lo, uint64_t hi) {


    if (lo >= hi) return;

    uint64_t pivot = array[(lo + hi) / 2]; // The value in the middle of the array

    // Left index
    uint64_t i = lo - 1;

    // Right index
    uint64_t j = hi + 1;

    while (1) {
        // Move the left index to the right at least once and while the element at
        // the left index is less than the pivot
        do {
            i += 1;
        } while (array[i] < pivot);

        // Move the right index to the left at least once and while the element at
        // the right index is greater than the pivot
        do {
            j -= 1;
        } while (array[j] > pivot);

        // If the indices crossed, return
        if (i >= j) {
            break;
        }

        // Swap the elements at the left and right indices
        uint64_t tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }

//    printf("%lu %lu %lu %lu \n", lo, j, j + 1, hi);
    printf("%lu p ",pivot);
    print_array(array, 16);
    quick_sort_correct(array, lo, j);
    quick_sort_correct(array, j + 1, hi);

}

void quick_sort(uint64_t *array, uint64_t size) {


    uint64_t *idx_pairs_0 = malloc(sizeof(uint64_t) * size * 2);
    idx_pairs_0[0] = 0;
    idx_pairs_0[1] = size - 1;

    uint64_t *idx_pairs_1 = malloc(sizeof(uint64_t) * size * 2);


    uint64_t idx_pairs_num = 2;
    uint64_t *idx_pairs = idx_pairs_0;
    uint64_t *idx_pairs_next = idx_pairs_1;

    while (idx_pairs_num > 0) {

        print_array(idx_pairs, idx_pairs_num);
        uint64_t idx_pairs_num_next = 0;

        for (uint64_t idx = 0; idx < idx_pairs_num; idx += 2) {

            uint64_t lo = idx_pairs[idx];
            uint64_t hi = idx_pairs[idx + 1];

//            if (lo < 0 || hi < 0 || lo >= hi) break;
            if (lo >= hi) break;

            uint64_t pivot = array[(lo + hi) / 2]; // The value in the middle of the array

            // Left index
            uint64_t i = lo - 1;

            // Right index
            uint64_t j = hi + 1;

            while (1) {
                // Move the left index to the right at least once and while the element at
                // the left index is less than the pivot
                do {
                    i += 1;
                } while (array[i] < pivot);

                // Move the right index to the left at least once and while the element at
                // the right index is greater than the pivot
                do {
                    j -= 1;
                } while (array[j] > pivot);

                // If the indices crossed, return
                if (i >= j) {

                    idx_pairs_next[idx_pairs_num_next] = lo;
                    idx_pairs_next[idx_pairs_num_next + 1] = j;
                    idx_pairs_next[idx_pairs_num_next + 2] = j + 1;
                    idx_pairs_next[idx_pairs_num_next + 3] = hi;

                    idx_pairs_num_next += 4;
                    break;
                }

                // Swap the elements at the left and right indices
                uint64_t tmp = array[i];
                array[i] = array[j];
                array[j] = tmp;
            }

//            printf("%lu p ",pivot);
//            print_array(array, 16);
        }

        idx_pairs_num = idx_pairs_num_next;

        if (idx_pairs_next == idx_pairs_0) {
            idx_pairs_next = idx_pairs_1;
            idx_pairs = idx_pairs_0;
        } else {
            idx_pairs_next = idx_pairs_0;
            idx_pairs = idx_pairs_1;
        }
    }

//    quick_sort(array, lo, j);
//    quick_sort(array, j + 1, hi);

}

int main(int argc, char **argv) {

    if (argc != 2) return 1;

    size_t size = atoi(argv[1]);
    uint64_t *array = generate_allocation(size, 100);
    print_array(array, size);

    struct timespec start;
    struct timespec end;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);

//    merge_sort(array, size);
    quick_sort(array, size);
//    quick_sort_correct(array, 0, size - 1);

    print_array(array, size);

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (double) (end.tv_sec - start.tv_sec);
    elapsed += (double) (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    check_array(array, size);

    //output of time measured in seconds
    printf("timed %s %.2fs\n", "func", elapsed);
    return 0;
}