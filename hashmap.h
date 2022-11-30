#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

typedef unsigned int HashFunction(char *str);

struct HashMapItem {
    char *key;
    void *value;
    struct HashMapItem *next;
};

struct HashMap {
    size_t size;
    struct HashMapItem **slots;
    HashFunction *hash;
};

struct HashMapItem* hm_item_new(char *key, void *value, struct HashMapItem *next);

void hm_item_free(struct HashMapItem *hm_item, void value_free(void *value));

struct HashMap* hm_new(size_t size);

struct HashMap* hm_new_with_hash(size_t size, HashFunction *hash);

void hm_free(struct HashMap *map, void value_free(void *value));

void hm_set(struct HashMap *map, char *key, void *value);

void *hm_get(struct HashMap *map, char *key);

/**
 * Remove and return the element with the matching key.
*/
void *hm_remove(struct HashMap *map, char *key);

void hm_foreach(struct HashMap *map, void callback(char *key, void *value));

void hm_set_hash(struct HashMap *map, HashFunction *hash);

#endif
