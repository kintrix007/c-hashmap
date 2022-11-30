#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

typedef unsigned int HashFunction(char *str);
typedef void ValueFreeFunction(void *value);

struct HashMapItem
{
    char *key;
    void *value;
    struct HashMapItem *next;
};

struct HashMap {
    size_t socket_count;
    struct HashMapItem **sockets;
    HashFunction *hash;
    ValueFreeFunction *value_free;
};

/**
 * Construct a new HashMap item.
*/
struct HashMapItem* hm_item_new(char *key, void *value, struct HashMapItem *next);

/**
 * Free all the memory allocated for the item.
 * @param value_free a function to call on the value.
 * This function should free the memory of the value.
 * Calls nothing if it is NULL.
*/
void hm_item_free(struct HashMapItem *hm_item, ValueFreeFunction value_free);

/**
 * Construct a new HashMap with a set amount of sockets.
 * @param socket_count the amount of sockets
 * @returns a pointer to the newly constructed HashMap
*/
struct HashMap* hm_new(size_t socket_count, ValueFreeFunction value_free);

/**
 * Construct a new HashMap with a set amount of sockets and a custom hash function.
 * @param socket_count the amount of sockets
 * @param hash the custom hash function
 * @returns a pointer to the newly constructed HashMap
 */
struct HashMap* hm_new_with_hash(size_t socket_count, ValueFreeFunction value_free, HashFunction *hash);

/**
 * Free all the memory allocated for the hashmap.
 * @param value_free a function to call on each value.
 * This function should free the memory of value.
 * Calls nothing if it is NULL.
*/
void hm_free(struct HashMap *map);

/**
 * Set a given key to correspond to a given value.
 * @param key if it already exists, then frees the previous value associated,
 * and sets it to the new one. Otherwise creates a new key-value item.
*/
void hm_set(struct HashMap *map, char *key, void *value);

/**
 * Get a value corresponding to the key in the HashMap.
 * @returns NULL if the key is not found, or a pointer to the value otherwise
*/
void *hm_get(struct HashMap *map, char *key);

/**
 * Remove and return the element with the matching key.
 * Does not modify the HashSet if the key does not exist.
 * @returns NULL if the key is not found, or a pointer to the value otherwise.
 * This HAS to be freed manually.
*/
void *hm_remove(struct HashMap *map, char *key);

/**
 * Iterate over all the key-value pairs of the HashMap.
 * @param callback a function that gets called with all the key-value pairs
*/
void hm_foreach(struct HashMap *map, void callback(char *key, void *value));

/**
 * Set the new hash function and rehash the keys.
*/
void hm_set_new_hash(struct HashMap *map, HashFunction *hash);

/**
 * Print the contents to stdout.
 * @param print_value a function to print a value from the HashMap
 */
void hm_print(struct HashMap *map, void print_value(void *value));

#endif
