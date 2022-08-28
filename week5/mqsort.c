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

uint64_t *merge_sort(uint64_t *array, uint64_t size) {


    // todo: assuming size is divisible by 2

    uint64_t unit = 2;
    uint64_t *tmp = malloc(size * sizeof(uint64_t));

    uint64_t *read_array = array;
    uint64_t *write_array = tmp;

    while (unit <= size) {

        uint64_t half = unit / 2;

        for (uint64_t idx = 0; idx < size; idx += unit) {


            uint64_t *left = read_array + idx;
            uint64_t *right = read_array + idx + half;
            uint64_t left_idx = 0;
            uint64_t right_idx = 0;


//            uint64_t *start = left;

            for (uint64_t i = 0; i < unit; i++) {

                if (left_idx == half) {
                    write_array[idx + i] = right[right_idx];
                    right_idx++;
                    continue;
                } else if (right_idx == half) {
                    write_array[idx + i] = left[left_idx];
                    left_idx++;
                    continue;
                }

                if (left[left_idx] < right[right_idx]) {
                    write_array[idx + i] = left[left_idx];
                    left_idx++;
                } else {
                    write_array[idx + i] = right[right_idx];
                    right_idx++;
                }

            }

//            printf("%lu-%lu ",left_idx,right_idx);

//            memcpy(start, tmp, sizeof(uint64_t) * unit);


        }

//        puts("");
        unit *= 2;


        uint64_t *copy = read_array;
        read_array = write_array;
        write_array = copy;
    }

    return read_array;
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
//    printf("%lu p ",pivot);
//    print_array(array, 16);
    quick_sort_correct(array, lo, j);
    quick_sort_correct(array, j + 1, hi);

}

uint64_t *quick_sort(uint64_t *array, uint64_t size) {


    uint64_t *idx_pairs_0 = malloc(sizeof(uint64_t) * size * 2);
    idx_pairs_0[0] = 0;
    idx_pairs_0[1] = size - 1;

    uint64_t *idx_pairs_1 = malloc(sizeof(uint64_t) * size * 2);


    uint64_t idx_pairs_num = 2;
    uint64_t *idx_pairs = idx_pairs_0;
    uint64_t *idx_pairs_next = idx_pairs_1;

    while (idx_pairs_num > 0) {

//        printf("%lu: ", idx_pairs_num);
//        print_array(idx_pairs, idx_pairs_num);
        uint64_t idx_pairs_num_next = 0;

        for (uint64_t idx = 0; idx < idx_pairs_num; idx += 2) {

            uint64_t lo = idx_pairs[idx];
            uint64_t hi = idx_pairs[idx + 1];

//            if (lo < 0 || hi < 0 || lo >= hi) break;
            if (lo >= hi) continue;

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

        uint64_t *tmp = idx_pairs_next;
        idx_pairs_next = idx_pairs;
        idx_pairs = tmp;
    }

//    quick_sort(array, lo, j);
//    quick_sort(array, j + 1, hi);

    return array;
}

int main(int argc, char **argv) {

    if (argc != 3) return 1;
    srand(time(NULL));

    size_t size = atoi(argv[1]);
    uint64_t *array = generate_allocation(size, 20);
//    print_array(array, size);

    struct timespec start;
    struct timespec end;
    double elapsed;

    char command[256];
    sprintf(command,
            "sudo /home/blepers/linux-huge/tools/perf/perf stat -e "
            "mem_load_retired.l1_hit,"
            "mem_load_retired.l1_miss,"
            "mem_load_retired.l2_hit,"
            "mem_load_retired.l2_miss,"
            "mem_load_retired.l3_hit,"
            "mem_load_retired.l3_miss"
            " -p %d &",
            getpid()
    );
    system(command);
    sleep(1);

    clock_gettime(CLOCK_MONOTONIC, &start);

    if (strcmp(argv[2], "quick") == 0) {
        array = quick_sort(array, size);
    } else {
        array = merge_sort(array, size);
    }


//    quick_sort_correct(array, 0, size - 1);

//    print_array(array, size);

    clock_gettime(CLOCK_MONOTONIC, &end);

    system("sudo killall -s INT -w perf");

    elapsed = (double) (end.tv_sec - start.tv_sec);
    elapsed += (double) (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    check_array(array, size);

    //output of time measured in seconds
    printf("timed %s %.2fs\n", argv[2], elapsed);
    return 0;
}