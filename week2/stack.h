//
// Created by Xiaoxiang Wu on 6/8/2022.
//

#ifndef SOFT3410_2022_STACK_H
#define SOFT3410_2022_STACK_H

#include "../main.h"

struct stack_node {
    void *val;
    struct stack_node *next;
};
struct stack {
    struct stack_node *top;
    size_t size;
};

void *stack_pop_object(struct stack *s);

void stack_push_copy(struct stack *s, void *data, size_t nbytes);

void *stack_peek(struct stack *s);

#endif //SOFT3410_2022_STACK_H
