//
// Created by xiaoxiang on 10/16/22.
//

#include "../main.h"

struct counter {
    char c;
    int count;

    struct counter *next;
};

struct counter *add(struct counter *curr, char c) {
    if (c != curr->c) {
        struct counter *new = malloc(sizeof(struct counter));
        new->c = c;
        new->count = 1;
        new->next = NULL;
        curr->next = new;

        return new;
    } else {
        curr->count++;

        return curr;
    }
}

int main() {

    char *data = "AAGGHHHHHHTTTT";

    unsigned long len = strlen(data);

    int num_thread = omp_get_max_threads();
    int per_thread = len / num_thread;
    int remainder = len % num_thread;

    struct counter **heads = malloc(sizeof(struct counter *) * num_thread);

#pragma omp parallel
    {
        int id = omp_get_thread_num();
//        char *buffer = malloc(sizeof(char) * per_thread);

        int start = id * per_thread;
        int end = (id + 1) * per_thread;

        if (id < remainder) {
            start += id;
            end += (id + 1);
        } else if (remainder) {
            start += remainder;
            end += remainder;
        }

        struct counter *head = malloc(sizeof(struct counter));
        head->c = data[start];
        head->next = NULL;
        head->count = 1;

        heads[id] = head;

        struct counter *curr = head;

        for (int i = start + 1; i != end; i++) {
            curr = add(curr, data[i]);
        }

        printf("id: %d start: %d end:%d\n", id, start, end);
    }

    printf("%lu-%d-%d ", len, num_thread, per_thread);

    struct counter *prev = NULL;

    for (int i = 0; i < num_thread; i++) {
        struct counter *curr = heads[i];

        while (curr != NULL) {

            if (prev != NULL) {

                if (prev->c == curr->c) {
                    curr->count += prev->count;
                } else {
                    printf("%d%c", prev->count, prev->c);
                }

                prev = NULL;
            }

            if (curr->next == NULL) {
                prev = curr;
            } else {
                printf("%d%c", curr->count, curr->c);
            }


            curr = curr->next;
        }
    }

    if (prev != NULL) {
        printf("%d%c", prev->count, prev->c);
    }

    printf("\n");

    return 0;
}