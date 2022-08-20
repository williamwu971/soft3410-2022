//
// Created by xiaoxiang on 8/20/22.
//

#include "../main.h"
#include <pthread.h>

struct safe_list_node {

    void *value;

    struct safe_list_node *next;
};

struct safe_list {

    struct safe_list_node *head;
    pthread_mutex_t mutex;
    pthread_spinlock_t spinlock;
};


static inline void safe_list_lock(struct safe_list *list) {

#ifdef SP
    pthread_spin_lock(&list->spinlock);
#else
    pthread_mutex_lock(&list->mutex);
#endif

}

static inline void safe_list_unlock(struct safe_list *list) {
#ifdef SP
    pthread_spin_unlock(&list->spinlock);
#else
    pthread_mutex_unlock(&list->mutex);
#endif
}

struct safe_list *safe_list_new() {

    struct safe_list *list = calloc(1, sizeof(struct safe_list));

    list->head = NULL;
    pthread_mutex_init(&list->mutex, NULL);
    pthread_spin_init(&list->spinlock, PTHREAD_PROCESS_PRIVATE);

    return list;
}

void safe_list_put(struct safe_list *list, void *value) {

    // todo: what's the speed of this?

    safe_list_lock(list);

    struct safe_list_node *node = calloc(1, sizeof(struct safe_list_node));
    node->value = value;
    node->next = list->head;
    list->head = node;

    safe_list_unlock(list);
}

void *safe_list_get(struct safe_list *list, int index) {

    safe_list_lock(list);

    struct safe_list_node *curr = list->head;
    for (int i = 0; i <= index; i++) {
        if (curr == NULL) break;
        curr = curr->next;
    }

    safe_list_unlock(list);

    return curr == NULL ? NULL : curr->value;
}

void *safe_list_remove(struct safe_list *list, int index) {

    safe_list_lock(list);

    struct safe_list_node *removed = NULL;
    if (index == 0 && list->head != NULL) {
        removed = list->head;
        list->head = removed->next;
    } else {
        struct safe_list_node *curr = list->head;
        for (int i = 0; i < index; i++) {
            if (curr == NULL) break;
            curr = curr->next;
        }

        if (curr->next != NULL) {
            removed = curr->next;
            curr->next = removed->next;
        }
    }

    safe_list_unlock(list);


    void *v = removed == NULL ? NULL : removed->value;
    free(removed);
    return v;
}

int bench_count;
int num_thread;

void *bench(void *arg) {

    struct safe_list *list = arg;
    int local_count = bench_count / num_thread;

    for (int i = 0; i < local_count; i++) {
        char *string = malloc(64 * sizeof(char));
        sprintf(string, "%d\n", i);
        safe_list_put(list, string);
    }
}


int fds[2];

void *hoarder() {

}

void *maker() {

}

int main(int argc, char **argv) {

    if (argc != 3) return 1;

    bench_count = atoi(argv[1]);
    num_thread = atoi(argv[2]);

    pthread_t *threads = malloc(num_thread * sizeof(pthread_t));
    struct safe_list *list = safe_list_new();

    declare_timer
    start_timer
    {
        for (int i = 0; i < num_thread; i++) {
            pthread_create(threads + i, NULL, bench, list);
        }
        for (int i = 0; i < num_thread; i++) {
            pthread_join(threads[i], NULL);
        }
    }stop_timer();

//    for (int i = 0; i < bench_count * 2; i++) {
//        if (safe_list_get(list, i) == NULL) {
//            printf("verified index at %d\n", i);
//            break;
//        }
//    }

    fprintf(stderr, "%f", bandwith(bench_count, elapsed));

    return 0;
}