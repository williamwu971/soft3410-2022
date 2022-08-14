//
// Created by Xiaoxiang Wu on 14/8/2022.
//

#include "dynamic_array_map.h"

struct dynamic_array_map_entry {
    void *key;
    void *value;
};

struct dynamic_array_map {
    size_t cap;
    size_t used;

    int (*cmp)(void *, void *);

    void (*keydel)(void *);

    void (*valdel)(void *);

    struct dynamic_array_map_entry *entries;
};

/**
* Allocates and return a new dynamic array object
* Must provide:
* Initial capacity
* Comparator function pointer,
* Key deletion function pointer,
* Value deletion function pointer
*/
struct dynamic_array_map *dynamic_array_map_new(size_t init_cap, int (*cmp)(void *, void *),
                                                void (*keydel)(void *), void(*valdel)(void *)) {

    struct dynamic_array_map *map = calloc(1, sizeof(struct dynamic_array_map));

    map->cap = init_cap;
    map->used = 0;
    map->cmp = cmp;
    map->keydel = keydel;
    map->valdel = valdel;

    map->entries = calloc(init_cap, sizeof(struct dynamic_array_map_entry));

    return map;
}

/**
* Given a key, the function should attempt tp place a value associated with the key.
* If the key does not exist, a new entry should be created and placed in the map
* If the key does exist, the old key should be deallocated
* except case: If old key and new key are the same address,
* no deallocation should occur
* If the key is NULL, no operations should occur
*/
void dynamic_array_map_put(struct dynamic_array_map *map, void *key, void *value) {
    for (size_t i = 0; i < map->used; i++) {
        if (!map->cmp(map->entries[i].key, key) && map->entries[i].key != key) {

            map->keydel(map->entries[i].key);
            // todo: free value?

            map->entries[i].key = key;
            map->entries[i].value = value;
            return;
        }
    }

    if (map->used == map->cap) {
        map->cap *= 2;
        map->entries = realloc(map->entries, map->cap * sizeof(struct dynamic_array_map_entry));
    }

    map->entries[map->used].key = key;
    map->entries[map->used].value = value;

    map->used++;
}

/*** Retrieves a value based on the key
* If the key is NULL, no value can be retrieved
* If the key is not null, the value mapped to the key should be retrieved
*/
void *dynamic_array_map_get(struct dynamic_array_map *map, void *key) {
    for (size_t i = 0; i < map->used; i++) {
        if (!map->cmp(map->entries[i].key, key)) {
            return map->entries[i].value;
        }
    }

    return NULL;
}

/**
* Removes the value based on the associated key
* If the key is NULL or the key does not map to any value, NULL should be returned
* If the key maps to a value within the data structure, the value should
* be removed (and not delaocated)
* The key and entry should be removed from the dynamic array
*
*/
void *dynamic_array_map_remove(struct dynamic_array_map *map, void *key) {


    void *v = NULL;
    size_t i;

    for (i = 0; i < map->used; i++) {
        if (!map->cmp(map->entries[i].key, key)) {

            map->keydel(map->entries[i].key);

            v = map->entries[i].value;
            break;
        }
    }

    if (i < map->used - 1) {
        map->used--;
        memmove(map->entries + i, map->entries + i + 1, sizeof(struct dynamic_array_map_entry) * (map->used - i));
    }

    return v;
}

/**
* Records the number of elements in the dynamic array
*/
size_t dynamic_array_map_size(struct dynamic_array_map *map) {
    return map->used;
}

/**
* Destroys the dynamic array
* Must remove all entries within the data structure and deallocate relevant keys
* and values
*
*/
void dynamic_array_map_destroy(struct dynamic_array_map *n) {
    for (size_t i = 0; i < n->used; i++) {
        n->keydel(n->entries[i].key);
        n->valdel(n->entries[i].value);
    }

    free(n->entries);
    free(n);
}

int main() {

    struct dynamic_array_map *map = dynamic_array_map_new(0, strcmp, free, free);

    void **keys = malloc(10 * sizeof(char *));
    void **vals = malloc(10 * sizeof(char *));

    for (int i = 0; i < 10; i++) {
        keys[i] = malloc(10 * sizeof(char));
        vals[i] = malloc(10 * sizeof(char));

        sprintf(keys[i], "key_%d", i);
        sprintf(vals[i], "val_%d", i);

        dynamic_array_map_put(map, keys[i], vals[i]);
        printf("key: %s val: %s\n", keys[i], dynamic_array_map_get(map, keys[i]));
    }

    printf("map size: %zu\n", dynamic_array_map_size(map));

    free(dynamic_array_map_remove(map, keys[0]));

    printf("map size: %zu\n", dynamic_array_map_size(map));

    dynamic_array_map_destroy(map);
}