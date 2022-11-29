#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "hashmap.h"

static unsigned int default_hash(char *str) {
    unsigned int hash = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        hash = str[i] + 31*hash;
    }
}

struct HashMap* hm_new(size_t size) {
    return hm_new_with_hash(size, default_hash);
}

struct HashMap* hm_new_with_hash(size_t size, HashFunction hash) {
    struct HashMap *map = malloc(sizeof(struct HashMap));
    
    map->size = size;
    map->slots = malloc(size * sizeof(struct HashMapItem*));
    for (int i = 0; i < size; i++) {
        map->slots[i] = NULL;
    }
    map->hash = hash;
    
    return map;
}

void hm_free(struct HashMap *map) {
    for (int i = 0; i < map->size; i++) {
        free(map->slots[i]);
    }
    free(map->slots);
    free(map);
}

void hm_set(struct HashMap *map, char *key, void *value);

void *hm_get(struct HashMap *map, char *key);

/**
 * Remove and return the element with the matching key.
 */
void *hm_remove(struct HashMap *map, char *key);

void hm_foreach(struct HashMap *map, void (*callback)(char *key, void *value));

void hm_set_hash(struct HashMap *map, HashFunction hash);
