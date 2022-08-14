//
// Created by Xiaoxiang Wu on 14/8/2022.
//

#include "dynamic_array_map.h"

struct dynamic_array_map_entry;
struct dynamic_array_map;

/**
* Allocates and return a new dynamic array object
* Must provide:
* Initial capacity
* Comparator function pointer,
* Key deletion function pointer,
* Value deletion function pointer
*/
struct dynamic_array_map *dynamic_array_map_new(size_t init_cap, int (*cmp)(void *, void *),
                                                void (*keydel)(void *), void(*valdel)(void *));

/**
* Given a key, the function should attempt tp place a value associated with the key.
* If the key does not exist, a new entry should be created and placed in the map
* If the key does exist, the old key should be deallocated
* except case: If old key and new key are the same address,
* no deallocation should occur
* If the key is NULL, no operations should occur
*/
void dynamic_array_map_put(struct dynamic_array_map *map, void *key, void *value);

/*** Retrieves a value based on the key
* If the key is NULL, no value can be retrieved
* If the key is not null, the value mapped to the key should be retrieved
*/
void *dynamic_array_map_get(struct dynamic_array_map *map, void *key);

/**
* Removes the value based on the associated key
* If the key is NULL or the key does not map to any value, NULL should be returned
* If the key maps to a value within the data structure, the value should
* be removed (and not delaocated)
* The key and entry should be removed from the dynamic array
*
*/
void *dynamic_array_map_remove(struct dynamic_array_map *map, void *key);

/**
* Records the number of elements in the dynamic array
*/
size_t dynamic_array_map_size(struct dynamic_array_map *map);

/**
* Destroyes the dynamic array
* Must remove all entries within the data structure and deallocate relevant keys
* and values
*
*/
void dynamic_array_map_destroy(struct dynamic_array_map *n);