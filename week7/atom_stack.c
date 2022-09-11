//
// Created by xiaoxiang on 9/11/22.
//

#include "atom_stack.h"

struct stack_node {
    void *data;
    atomic_int_fast64_t next;
};
struct stack {

    atomic_int size;
    atomic_int_fast64_t head;
};

struct stack *stack_new() {

    assert(sizeof(struct stack_node *) == sizeof(atomic_int_fast64_t));

    struct stack *s = malloc(sizeof(struct stack));
    atomic_store(&s->size, 0);
    atomic_store(&s->head, 0);

    return s;
}

void stack_push(struct stack *s, void *data) {
    struct stack_node *sn = malloc(sizeof(struct stack));

    sn->data = data;
    sn->next = atomic_load(&s->head);

    while (!atomic_compare_exchange_strong(&s->head, &sn->next, sn)) {

    }
}

void *stack_pop(struct stack *s) {

    atomic_int_fast64_t head = atomic_load(&s->head);
    while (!atomic_compare_exchange_strong(&s->head, &head, ((struct stack_node *) head)->next)) {

    }
    void *data = ((struct stack_node *) head)->data;
    free((void *) head);

    return data;
}

void stack_destroy(struct stack *s) {
    free(s);
}

