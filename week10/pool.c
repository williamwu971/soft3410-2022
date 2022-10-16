#include "../main.h"

#define MAIN_THREAD_TIMEOUT (60)
#define N_JOBS (100)

struct thread_job {

    void (*fn)(void *);

    void *data;

    struct thread_job *next;
};

struct thread_queue {
    int stopped;
    pthread_t *thread_ids;
    size_t num_thread;
//    pthread_cond_t haswork;

    sem_t take_work;

    pthread_mutex_t lock;
    struct thread_job *work;
//    int num_work;
};

void *thread_queue_work(void *arg) {
    struct thread_queue *tp = (struct thread_queue *) arg;

    while (!tp->stopped) {

        struct thread_job *job = NULL;


        sem_wait(&tp->take_work);
        pthread_mutex_lock(&tp->lock);


        if (tp->work != NULL) {

            // take work
            job = tp->work;
            tp->work = tp->work->next;
        }

        pthread_mutex_unlock(&tp->lock);

        if (job) {
            job->fn(job->data);
            free(job);
        }
    }
}

struct thread_queue *thread_queue_new(size_t n) {

    struct thread_queue *tp = malloc(sizeof(struct thread_queue));

    sem_init(&tp->take_work, 0, 0);
    pthread_mutex_init(&tp->lock, NULL);
    tp->stopped = 0;
    tp->work = NULL;
    tp->num_thread = n;

    tp->thread_ids = malloc(sizeof(pthread_t) * n);
    for (size_t i = 0; i < n; i++) {
        pthread_create(tp->thread_ids + i, NULL, thread_queue_work, tp);
    }

    return tp;
}

void thread_queue_enqueue(struct thread_queue *pool, void(*fn)(void *), void *data) {

    struct thread_job *new_job = malloc(sizeof(struct thread_job));
    new_job->data = data;
    new_job->fn = fn;
    new_job->next = NULL;

    pthread_mutex_lock(&pool->lock);

    if (pool->work == NULL) {
        pool->work = new_job;
    } else {
        struct thread_job *current = pool->work;

        while (current->next != NULL) {
            current = current->next;
        }

        current->next = new_job;
    }

    pthread_mutex_unlock(&pool->lock);
//    pthread_cond_signal(&pool->haswork);
    sem_post(&pool->take_work);
}

int thread_job_is_equal(struct thread_job *job_a, struct thread_job *job_b) {
    return (job_a->fn == job_b->fn) && (job_a->data == job_b->data);
}

void thread_queue_deque(struct thread_queue *pool, void(*fn)(void *), void *data) {

    struct thread_job job;
    job.fn = fn;
    job.data = data;

    struct thread_job *to_free = NULL;

    pthread_mutex_lock(&pool->lock);

    if (thread_job_is_equal(pool->work, &job)) {
        to_free = pool->work;
        pool->work = pool->work->next;
    } else {
        struct thread_job *current = pool->work;

        while (current->next != NULL) {

            if (thread_job_is_equal(current->next, &job)) {
                to_free = current->next;
                current->next = current->next->next;
                break;
            }
            current = current->next;
        }
    }

    pthread_mutex_unlock(&pool->lock);

    if (to_free) {
        free(to_free);
    }
}

int thread_queue_empty(struct thread_queue *q) {
    pthread_mutex_lock(&q->lock);

    int res = (q->work == NULL);

    pthread_mutex_unlock(&q->lock);

    return res;
}

void thread_queue_destroy(struct thread_queue *pool) {

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

    if (pool->work) {
        puts("oops, work remained");

        struct thread_job *curr = pool->work;
        struct thread_job *next = NULL;

        while (curr != NULL) {
            next = curr->next;
            free(curr);
            curr = next;
        }
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
}

int main() {
    struct thread_queue *pool = thread_queue_new(8);
    struct work_data datas[N_JOBS];
    for (size_t i = 0; i < N_JOBS; i++) {
        struct work_data *d = datas + i;
        d->time = 1;
        d->work_id = i;
        thread_queue_enqueue(pool, mysleep, d);
    }

    while (!thread_queue_empty(pool)) {
        sleep(1);
    }

    thread_queue_destroy(pool);
    puts("Finishing up now!");
    return 0;
}