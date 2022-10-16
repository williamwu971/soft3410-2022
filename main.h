//
// Created by Xiaoxiang Wu on 5/8/2022.
//

#ifndef SOFT3410_2022_MAIN_H
#define SOFT3410_2022_MAIN_H

#define _GNU_SOURCE


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <sched.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <poll.h>
#include <omp.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <sys/mman.h>

static __uint128_t g_lehmer64_state;

static void init_seed(void) {
    srand(time(NULL));
    g_lehmer64_state = rand();
}

static uint64_t lehmer64() {
    g_lehmer64_state *= 0xda942042e4dd58b5;
    return g_lehmer64_state >> 64;
}

/* data_size in Bytes, time in us, returns bandwith in MB/s */
static double bandwith(long ops, long time) {
    return (((double) ops)) / (((double) time) / 1000000.);
}

#define declare_timer uint64_t elapsed; \
   struct timeval st, et;

#define start_timer gettimeofday(&st,NULL);

#define stop_timer(msg, args...) ;do { \
   gettimeofday(&et,NULL); \
   elapsed = ((et.tv_sec - st.tv_sec) * 1000000) + (et.tv_usec - st.tv_usec) + 1; \
   printf("(%s,%d) [%6lums] " msg "\n", __FUNCTION__ , __LINE__, elapsed/1000, ##args); \
} while(0)

#define die(msg, args...) \
   do {                         \
      fprintf(stderr,"(%s,%d,%s) " msg "\n", __FUNCTION__ , __LINE__,strerror(errno), ##args); \
      fflush(stderr);fflush(stdout); \
      exit(-1); \
   } while(0)

#define T_declare_timer struct timespec start;struct timespec end;double elapsed

#define T_start_timer do {clock_gettime(CLOCK_MONOTONIC, &start);} while (0)

#define T_stop_timer(msg, args...) ;do { \
   clock_gettime(CLOCK_MONOTONIC, &end); \
   elapsed = (double) (end.tv_sec - start.tv_sec); \
   elapsed += (double) (end.tv_nsec - start.tv_nsec) / 1000000000.0; \
   printf("(%s,%d) [%6.2fs] " msg "\n", __FUNCTION__ , __LINE__, elapsed, ##args); \
} while(0)

#define pin(id) do{ \
    cpu_set_t cpu; \
    CPU_ZERO(&cpu); \
    CPU_SET(id, &cpu); \
    pthread_t self = pthread_self(); \
    pthread_setaffinity_np(self,sizeof(cpu_set_t),&cpu);\
}while (0)

#if defined(__x86_64__)
#define declare_parallel(N) \
    pthread_attr_t attrs[N];    \
    cpu_set_t cpus[N];\
    pthread_t threads[N];   \
    void* returns[N];\
    for (int C = 0; C < (N); C++) {\
        CPU_ZERO(cpus + C);\
        CPU_SET(C + 1, cpus + C);\
        pthread_attr_init(attrs + C);\
        pthread_attr_setaffinity_np(attrs + C, sizeof(cpu_set_t), cpus + C);\
    }
#else
#define declare_parallel(N) \
    pthread_attr_t attrs[N];    \
    pthread_t threads[N];   \
    void* returns[N];\
    for (int C = 0; C < (N); C++) {\
        pthread_attr_init(attrs + C);\
    }
#endif

#define run_parallel(N, ROUTINE) do{ \
    for (int C = 0; C < (N); C++) {\
        pthread_create(threads + C, attrs + C, ROUTINE, NULL);\
    }                                \
    for (int C = 0; C < (N); C++) {  \
        pthread_join(threads[C], returns+C);\
    }                            \
}while(0)

#define shuffle(array, n, size) do {\
    if ((n) <= 1) break;\
    srand(time(NULL));\
\
    size_t i;\
    void *buffer = malloc(size);\
\
    for (i = 0; i < (n) - 1; i++) {\
        size_t j = i + rand() / (RAND_MAX / ((n) - i) + 1);\
\
        memcpy(buffer, (array) + j * (size), size);\
        memcpy((array) + j * (size), (array) + i * (size), size);\
        memcpy((array) + i * (size), buffer, size);\
    }\
\
}while(0)

#endif //SOFT3410_2022_MAIN_H
