//
// Created by Xiaoxiang Wu on 5/8/2022.
//

#include "linkedlist_map.h"

struct linkedlist_map_entry {

    void *key;
    void *value;
    struct linkedlist_map_entry *next;
};


struct linkedlist_map {

    int (*cmp)(void *, void *);

    void (*keydel)(void *);

    void (*valdel)(void *);

    struct linkedlist_map_entry *head;

    size_t size;
};

struct linkedlist_map *linkedlist_map_new(int (*cmp)(void *, void *),
                                          void (*keydel)(void *), void(*valdel)(void *)) {
    struct linkedlist_map *map = malloc(sizeof(struct linkedlist_map));

    map->cmp = cmp;
    map->keydel = keydel;
    map->valdel = valdel;
    map->head = NULL;
    map->size = 0;

    return map;
}

struct linkedlist_map_entry *linkedlist_map_new_entry(void *key, void *value) {

    struct linkedlist_map_entry *entry = malloc(sizeof(struct linkedlist_map_entry));

    entry->key = key;
    entry->value = value;
    entry->next = NULL;

    return entry;

}

void linkedlist_map_put(struct linkedlist_map *map, void *key, void *value) {

    if (map->head == NULL) {
        map->head = linkedlist_map_new_entry(key, value);
        return;
    }

    struct linkedlist_map_entry *curr = map->head;

    while (1) {

        if (map->cmp(curr->key, key) == 0) return;

        if (curr->next != NULL) {
            curr = curr->next;
        } else {
            break;
        }
    }

    curr->next = linkedlist_map_new_entry(key, value);

}

void *linkedlist_map_get(struct linkedlist_map *map, void *key) {

    struct linkedlist_map_entry *curr = map->head;

    while (curr != NULL) {

        if (map->cmp(key, curr->key) == 0) return curr->value;

        curr = curr->next;
    }

    return NULL;
}

void *linkedlist_map_remove(struct linkedlist_map *map, void *key) {


    if (map->head != NULL && map->cmp(key, map->head->key) == 0) {
        map->head = map->head->next;
    }

    struct linkedlist_map_entry *curr = map->head;

    while (curr->next != NULL) {

        if (map->cmp(key, curr->next->key) == 0) {
            map->keydel(curr->next->key);
            void *value = curr->next->value;

            // connect
            curr->next = curr->next->next;

            free(curr->next);

            map->size--;

            return value;
        }

        curr = curr->next;
    }

    return NULL;

}

size_t linkedlist_map_size(struct linkedlist_map *map) {

    return map->size;
}

void linkedlist_map_destroy(struct linkedlist_map *n) {

    struct linkedlist_map_entry *curr = n->head;

    while (curr != NULL) {


        struct linkedlist_map_entry *next = curr->next;

        n->keydel(curr->key);
        n->valdel(curr->value);
        free(curr);

        curr = next;
    }

    free(n);
}


int (cmp)(void *a, void *b) {
    return ((u_int64_t *) a)[0] != ((u_int64_t *) b)[0];
}

void (keydel)(void *k) {

}

void (valdel)(void *v) {

}

int main() {

    u_int64_t keys[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    char *values[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};

    struct linkedlist_map *map = linkedlist_map_new(cmp, keydel, valdel);

    for (int i = 0; i < 9; i++) {
        linkedlist_map_put(map, keys + i, values[i]);
    }

    for (int i = 0; i < 9; i++) {
        printf("key %llu maps tp %s\n", keys[i], (char *) linkedlist_map_get(map, keys + i));
    }

    return 0;
}