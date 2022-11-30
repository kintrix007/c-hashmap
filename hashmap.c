#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hashmap.h"

static unsigned int default_hash(char *str) {
    unsigned int hash = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        hash = str[i] + 31*hash;
    }
    
    return hash;
}

static void default_print_value(void *value) {
    if (value == NULL) {
        printf("(nil)");
    } else {
        printf("%d", *(int *)value);
    }
}

struct HashMapItem *hm_item_new(char *key, void *value, struct HashMapItem *next) {
    struct HashMapItem *item = malloc(sizeof(struct HashMapItem));
    
    item->key = malloc(strlen(key)+1);
    strcpy(item->key, key);
    item->value = value;
    item->next = next;

    return item;
}

void hm_item_free(struct HashMapItem *hm_item, void value_free(void *value)) {
    if (value_free != NULL) value_free(hm_item->value);
    free(hm_item->key);
    free(hm_item);
}

struct HashMap* hm_new(size_t sockets) {
    return hm_new_with_hash(sockets, default_hash);
}

struct HashMap* hm_new_with_hash(size_t sockets, HashFunction hash) {
    struct HashMap *map = malloc(sizeof(struct HashMap));
    
    map->sockets = sockets;
    map->slots = malloc(sockets * sizeof(struct HashMapItem*));
    for (size_t i = 0; i < sockets; i++) {
        map->slots[i] = NULL;
    }
    map->hash = hash;
    
    return map;
}

void hm_free(struct HashMap *map, void value_free(void *value)) {
    assert(map != NULL);

    for (size_t i = 0; i < map->sockets; i++) {
        struct HashMapItem *item = map->slots[i];

        while (item != NULL) {
            struct HashMapItem *tmp = item->next;
            hm_item_free(item, value_free);
            item = tmp;
        }
    }

    free(map->slots);
    free(map);
}

void hm_set(struct HashMap *map, char *key, void *value) {
    assert(map != NULL);
    
    int idx = map->hash(key) % map->sockets;
    struct HashMapItem *item = map->slots[idx];

    if (item == NULL) {
        map->slots[idx] = hm_item_new(key, value, NULL);
        return;
    }

    while (item->next != NULL && strcmp(item->key, key) != 0) {
        item = item->next;
    }
    assert(item != NULL);

    if (strcmp(item->key, key) == 0) {
        item->value = value;
    } else {
        assert(item->next == NULL);
        item->next = hm_item_new(key, value, NULL);
    }
}

void *hm_get(struct HashMap *map, char *key) {
    assert(map != NULL);

    int idx = map->hash(key) % map->sockets;
    struct HashMapItem *item = map->slots[idx];

    if (item == NULL) return NULL;

    while (item->next != NULL && strcmp(item->key, key) != 0) {
        item = item->next;
    }
    assert(item != NULL);

    if (strcmp(item->key, key) != 0) return NULL;

    return item->value;
}

void *hm_remove(struct HashMap *map, char *key) {
    assert(map != NULL);

    int idx = map->hash(key) % map->sockets;
    struct HashMapItem *item = map->slots[idx];
    struct HashMapItem *prev = NULL;

    if (item == NULL) return NULL;

    while (item->next != NULL && strcmp(item->key, key) != 0) {
        prev = item;
        item = item->next;
    }
    assert(item != NULL);

    if (strcmp(item->key, key) != 0) return NULL;

    if (prev == NULL) {
        //? It just gets set to NULL if it is the only item in the socket
        map->slots[idx] = item->next;
    } else {
        prev->next = item->next;
    }

    void *value = item->value;
    hm_item_free(item, NULL);
    return value;
}

void hm_foreach(struct HashMap *map, void callback(char *key, void *value)) {
    assert(map != NULL);
    if (callback == NULL) return;

    for (size_t i = 0; i < map->sockets; i++) {
        struct HashMapItem *item = map->slots[i];

        while (item != NULL) {
            callback(item->key, item->value);
            item = item->next;
        }
    }
}

void hm_set_new_hash(struct HashMap *map, HashFunction hash);

void hm_print(struct HashMap *map, void print_value(void *value)) {
    assert(map != NULL);

    if (print_value == NULL) print_value = default_print_value;

    printf("====START====\n");
    for (size_t i = 0; i < map->sockets; i++) {
        struct HashMapItem *item = map->slots[i];
        if (item == NULL) continue;

        printf("Socket #%05lu: ", i);

        while (item != NULL) {
            printf("\"%s\" : ", item->key);
            print_value(item->value);

            if (item->next != NULL) printf(", ");
            item = item->next;
        }
        printf("\n");
    }
    printf("=====END=====\n");
}
