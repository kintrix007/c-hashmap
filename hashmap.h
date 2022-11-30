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

/**
 * Construct a new HashMap with a set amount of sockets.
 * @param size the amount of sockets
 * @returns a pointer to the newly constructed HashMap
*/
struct HashMap* hm_new(size_t size);

/**
 * Construct a new HashMap with a set amount of sockets and a custom hash function.
 * @param size the amount of sockets
 * @param hash the custom hash function
 * @returns a pointer to the newly constructed HashMap
 */
struct HashMap* hm_new_with_hash(size_t size, HashFunction *hash);

/**
 * Free all the memory allocated for the hashmap.
*/
void hm_free(struct HashMap *map, void value_free(void *value));

/**
 * Set a given key to a new value in the HashMap.
*/
void hm_set(struct HashMap *map, char *key, void *value);

/**
 * Get a value corresponding to the key in the HashMap.
 * @returns NULL if the key does not exist, a pointer to the value otherwise
*/
void *hm_get(struct HashMap *map, char *key);

/**
 * Remove and return the element with the matching key.
 * Does not modify the HashSet if the key does not exist.
*/
void *hm_remove(struct HashMap *map, char *key);

void hm_foreach(struct HashMap *map, void callback(char *key, void *value));

void hm_set_hash(struct HashMap *map, HashFunction *hash);

#endif
