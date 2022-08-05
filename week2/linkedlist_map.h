//
// Created by Xiaoxiang Wu on 5/8/2022.
//

#ifndef SOFT3410_2022_LINKEDLIST_MAP_H
#define SOFT3410_2022_LINKEDLIST_MAP_H


#include "../main.h"

struct linkedlist_map_entry;
struct linkedlist_map;

struct linkedlist_map *linkedlist_map_new(int (*cmp)(void *, void *),
                                          void (*keydel)(void *), void(*valdel)(void *));

void linkedlist_map_put(struct linkedlist_map *map, void *key, void *value);

void *linkedlist_map_get(struct linkedlist_map *map, void *key);

void *linkedlist_map_remove(struct linkedlist_map *map, void *key);

size_t linkedlist_map_size(struct linkedlist_map *map);

void linkedlist_map_destroy(struct linkedlist_map *n);

#endif //SOFT3410_2022_LINKEDLIST_MAP_H
