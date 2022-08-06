//
// Created by Xiaoxiang Wu on 6/8/2022.
//

#include "stack.h"


struct stack *stack_new() {

    struct stack *s = malloc(sizeof(struct stack));

    s->size = 0;
    s->top = NULL;

    return s;

}

void *stack_pop_object(struct stack *s) {

    struct stack_node *sn = s->top;
    if (s->top)s->top = s->top->next;

    return sn->val;

}

void stack_push_copy(struct stack *s, void *data, size_t nbytes) {


    void *data_copy = malloc(nbytes);
    memcpy(data_copy, data, nbytes);

//    puts(data);
//    puts(data_copy);

    struct stack_node *sn = malloc(sizeof(struct stack_node));
    sn->next = s->top;
    sn->val = data_copy;

    s->top = sn;

}

void *stack_peek(struct stack *s) {

    return s == NULL ? NULL : s->top->val;
}


int main() {

    char *values[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};

    struct stack *s = stack_new();

    for (int i = 0; i < 9; i++) {
        stack_push_copy(s, values[i], 2);
        printf("stack peek: %s\n", stack_peek(s));
    }

    puts("");

    for (int i = 0; i < 9; i++) {
        printf("stack pop: %s\n", stack_pop_object(s));
    }

    return 0;

}