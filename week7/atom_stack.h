//
// Created by xiaoxiang on 9/11/22.
//

#ifndef SOFT3410_2022_ATOM_STACK_H
#define SOFT3410_2022_ATOM_STACK_H

#include "../main.h"

struct stack_node;
struct stack;

/**
* @return stack, returns a new stack object on the heap
*/
struct stack *stack_new();

/**
* Pushes a new object on the stack, this will be at the
* top of the stack.
* @param stack
* @param data
*/
void stack_push(struct stack *, void *data);

/**
* Pops the last element from the top of the stack.
* @param stack
* @return object, object at the top of the stack
*/
void *stack_pop(struct stack *);

/**
* Destroys the stack, deallocates any memory associated with it.
* @param stack
*/
void stack_destroy(struct stack *);

#endif //SOFT3410_2022_ATOM_STACK_H
