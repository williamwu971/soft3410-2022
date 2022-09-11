//
// Created by xiaoxiang on 9/11/22.
//

#ifndef SOFT3410_2022_REF_H
#define SOFT3410_2022_REF_H

#include "../main.h"

/**
* Request an allocation of memory based on the size parameter
* and specify if it is dependency of another reference. If the dep argument
* is NULL then it is a root and does not depend on any other object.
*/
void *new_ref(size_t size, void *dep);

/**
* Increments the reference count for the given reference.
* If the reference is not valid, NULL should be returned
* If the reference is valid, the same reference should be returned
* and the counter incremented
* If NULL is passed, NULL should be returned and no increment operation occurs
*/
void *inc_ref(void *ref);

/**
* Decrements the reference count for a given reference.
* If the reference is not valid or NULL, NULL should be returned
* and no decrement operation should occur.
* If the reference is valid, the reference should be decremented,
*
If the counter is 0, the allocation should be deallocated
*
and NULL returned.
*/
void *dec_ref(void *ref, int deep);

#endif //SOFT3410_2022_REF_H
