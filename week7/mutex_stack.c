//
// Created by xiaoxiang on 9/11/22.
//

#include "atom_stack.h"

struct stack_node {
    void *data;
    struct stack_node *next;
};
struct stack {

    int size;
    struct stack_node *head;
    pthread_mutex_t lock;
};

struct stack *stack_new() {


    struct stack *s = malloc(sizeof(struct stack));
    s->size = 0;
    s->head = NULL;
    pthread_mutex_init(&s->lock, NULL);

    return s;
}

void stack_push(struct stack *s, void *data) {
    struct stack_node *sn = malloc(sizeof(struct stack));

    sn->data = data;

    pthread_mutex_lock(&s->lock);
    sn->next = s->head;
    s->head = sn;
    pthread_mutex_unlock(&s->lock);

}

void *stack_pop(struct stack *s) {

    pthread_mutex_lock(&s->lock);
    struct stack_node *sn = s->head;
    s->head = sn->next;
    pthread_mutex_unlock(&s->lock);

    void *data = sn->data;
    free(sn);

    return data;
}

void stack_destroy(struct stack *s) {
    free(s);
}