//
// Created by xiaoxiang on 10/16/22.
//

#define _GNU_SOURCE

#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>


int main(int argc, char **argv) {

    long len = 16 * 1024 * 1024L;
    long nb_accesses = 1000000;
    long granularity = 64;

    int fd = open("disk_pool", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    assert(lseek(fd, len, SEEK_SET) == len);
    assert(write(fd, "a", 1) != -1);

    char *map = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED_VALIDATE, fd, 0);
    assert(map != MAP_FAILED);
    close(fd);

    memset(map, 0, len);
    long *locations = malloc(nb_accesses * sizeof(long));
    srand(time(NULL));

    if (argc != 1 && strcasestr(argv[1], "r")) {
        puts("random");

        for (long i = 0; i < nb_accesses; i++) {
            int loc = rand() % (len - granularity);
            loc = loc / granularity * granularity;
            locations[i] = loc;
        }
    } else {
        puts("sequential");
        for (long i = 0; i < nb_accesses; i++) {
            locations[i] = i * granularity;
        }
    }

    // data
    char *data = malloc(sizeof(char) * granularity);
    for (long i = 0; i < granularity; i++) {
        data[i] = rand();
    }

    omp_set_num_threads(8);

    u_int64_t elapsed;
    struct timeval st, et;

    gettimeofday(&st, NULL);

#pragma omp parallel
    {

        long start = nb_accesses / omp_get_max_threads() * omp_get_thread_num();
        long end = nb_accesses / omp_get_max_threads() * (omp_get_thread_num() + 1);

        printf("thread %d start %ld end %ld\n", omp_get_thread_num(), start, end);

        for (long i = start; i != end; i++) {
            long loc = locations[i];
            memcpy(map + loc, data, granularity);
            msync(map + loc, granularity, MS_SYNC);
        }
    }


    gettimeofday(&et, NULL);
    elapsed = ((et.tv_sec - st.tv_sec) * 1000000) + (et.tv_usec - st.tv_usec) + 1;
    printf("(%s,%d) [%6lums] Doing %ld memcpy of %ld bytes (%f MB/s)\n", __FUNCTION__, __LINE__, elapsed / 1000,
           nb_accesses, granularity,
           ((double) (nb_accesses * granularity) / 1024. / 1024.) / (((double) elapsed) / 1000000.));
}