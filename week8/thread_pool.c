//
// Created by xiaoxiang on 9/18/22.
//

#include "../main.h"

#define MAIN_THREAD_TIMEOUT (60)
#define N_JOBS (200)

struct thread_data {
    void (*fn)(void *);

    void *data;
};

struct thread_pool {
    int stopped;
    pthread_t *thread_ids;
    size_t num_thread;
//    pthread_cond_t haswork;

    sem_t take_work;

    pthread_mutex_t lock;
    struct thread_data *work;
    int num_work;
};

//struct thread_data *thread_data_new(void(*fn)(void *), void *data) {
//    struct thread_data *td = malloc(sizeof(struct thread_data));
//    td->fn = fn;
//    td->data = data;
//}
//
//void thread_data_destroy(struct thread_data *j) {
//
//}

void *thread_pool_work(void *arg) {
    struct thread_pool *tp = (struct thread_pool *) arg;

    while (!tp->stopped) {

        void (*fn)(void *) = NULL;
        void *data = NULL;
        int exec = 0;


        sem_wait(&tp->take_work);
        pthread_mutex_lock(&tp->lock);


        if (tp->num_work > 0) {

            // take work
            fn = tp->work[tp->num_work - 1].fn;
            data = tp->work[tp->num_work - 1].data;
            exec = 1;

            // dequeue work
            tp->num_work--;
            tp->work = realloc(tp->work, tp->num_work * sizeof(struct thread_data));
        }

        pthread_mutex_unlock(&tp->lock);

        if (exec) {
            fn(data);
        }
    }
}

struct thread_pool *thread_pool_new(size_t n) {

    struct thread_pool *tp = malloc(sizeof(struct thread_pool));

    sem_init(&tp->take_work, 0, 0);
    pthread_mutex_init(&tp->lock, NULL);
    tp->stopped = 0;
    tp->work = NULL;
    tp->num_work = 0;
    tp->num_thread = n;

    tp->thread_ids = malloc(sizeof(pthread_t) * n);
    for (size_t i = 0; i < n; i++) {
        pthread_create(tp->thread_ids + i, NULL, thread_pool_work, tp);
    }

    return tp;
}

void thread_pool_execute(struct thread_pool *pool, void(*fn)(void *), void *data) {
    pthread_mutex_lock(&pool->lock);

    pool->num_work++;
    pool->work = realloc(pool->work, pool->num_work * sizeof(struct thread_data));
    pool->work[pool->num_work - 1].data = data;
    pool->work[pool->num_work - 1].fn = fn;

    pthread_mutex_unlock(&pool->lock);
//    pthread_cond_signal(&pool->haswork);
    sem_post(&pool->take_work);
}

void thread_pool_destroy(struct thread_pool *pool) {

    puts("destroy!");

    pool->stopped = 1;

    for (size_t i = 0; i < pool->num_thread; i++) {
        sem_post(&pool->take_work);
    }

    for (size_t i = 0; i < pool->num_thread; i++) {
        pthread_join(pool->thread_ids[i], NULL);
    }

    pthread_mutex_destroy(&pool->lock);
    sem_destroy(&pool->take_work);

    free(pool->thread_ids);

    if (pool->num_work > 0) {
        printf("oops, %d work remained\n", pool->num_work);
        free(pool->work);
    }

    free(pool);
}


struct work_data {
    uint32_t time;
    uint32_t work_id;
};

void mysleep(void *arg) {
    struct work_data *data = (struct work_data *) arg;
    printf("is working on job %u\n", data->work_id);
    fflush(stdout);
    sleep(data->time);
    free(data);
}

int main() {
    struct thread_pool *pool = thread_pool_new(8);
    for (size_t i = 0; i < N_JOBS; i++) {
        struct work_data *d = malloc(sizeof(struct work_data));
        d->time = 1;
        d->work_id = i;
//        struct thread_data *job = thread_data_new();
        thread_pool_execute(pool, mysleep, d);
    }
    sleep(MAIN_THREAD_TIMEOUT);
    thread_pool_destroy(pool);
    puts("Finishing up now!");
    return 0;
}