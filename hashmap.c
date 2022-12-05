#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hashmap.h"

static unsigned int default_hash(char *str) {
    unsigned int hash = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        hash = str[i] + 31 * hash;
    }

    return hash;
}

static void default_print_value(void *value) {
    if (value == NULL) {
        printf("(nil)");
    } else {
        printf("%d", *(int*)value);
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

void hm_item_free(struct HashMapItem *hm_item, ValueFreeFunction value_free) {
    if (hm_item != NULL && value_free != NULL) value_free(hm_item->value);
    free(hm_item->key);
    free(hm_item);
}


inline struct HashMap* hm_new(size_t socket_count, ValueFreeFunction value_free) {
    return hm_new_with_hash(socket_count, value_free, default_hash);
}

struct HashMap* hm_new_with_hash(size_t socket_count, ValueFreeFunction value_free, HashFunction hash) {
    assert(hash != NULL);

    struct HashMap *map = malloc(sizeof(struct HashMap));
    
    map->size = 0;
    map->socket_count = socket_count;
    map->sockets = malloc(socket_count * sizeof(struct HashMapItem*));
    for (size_t i = 0; i < socket_count; i++) {
        map->sockets[i] = NULL;
    }
    map->hash = hash;
    map->value_free = value_free;
    
    return map;
}

void hm_free(struct HashMap *map) {
    assert(map != NULL);

    for (size_t i = 0; i < map->socket_count; i++) {
        struct HashMapItem *item = map->sockets[i];

        while (item != NULL) {
            struct HashMapItem *tmp = item->next;
            hm_item_free(item, map->value_free);
            item = tmp;
        }
    }

    free(map->sockets);
    free(map);
}

void hm_set(struct HashMap *map, char *key, void *value) {
    assert(map != NULL);
    assert(key != NULL); //? There is no good way to make NULL keys fail silently.
    
    int idx = map->hash(key) % map->socket_count;
    struct HashMapItem *item = map->sockets[idx];

    if (item == NULL) {
        map->sockets[idx] = hm_item_new(key, value, NULL);
        map->size++;
        return;
    }

    while (item->next != NULL && strcmp(item->key, key) != 0) {
        item = item->next;
    }
    assert(item != NULL);

    if (strcmp(item->key, key) == 0) {
        map->value_free(item->value);
        item->value = value;
    } else {
        assert(item->next == NULL);
        item->next = hm_item_new(key, value, NULL);
        map->size++;
    }
}

void *hm_get(struct HashMap *map, char *key) {
    assert(map != NULL);

    int idx = map->hash(key) % map->socket_count;
    struct HashMapItem *item = map->sockets[idx];

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

    int idx = map->hash(key) % map->socket_count;
    struct HashMapItem *item = map->sockets[idx];
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
        map->sockets[idx] = item->next;
    } else {
        prev->next = item->next;
    }
    map->size--;

    void *value = item->value;
    hm_item_free(item, NULL);
    return value;
}

char **hm_get_keys(struct HashMap *map) {
    assert(map != NULL);

    size_t idx = 0;
    char **keys = malloc(map->size * sizeof(char *));

    for (size_t i = 0; i < map->socket_count; i++) {
        struct HashMapItem *item = map->sockets[i];

        while (item != NULL) {
            keys[idx++] = item->key;
            item = item->next;
        }
    }

    return keys;
}

void **hm_get_values(struct HashMap *map) {
    assert(map != NULL);

    size_t idx = 0;
    void **values = malloc(map->size * sizeof(void *));

    for (size_t i = 0; i < map->socket_count; i++) {
        struct HashMapItem *item = map->sockets[i];

        while (item != NULL) {
            values[idx++] = item->value;
            item = item->next;
        }
    }

    return values;
}

void hm_foreach(struct HashMap *map, void callback(char *key, void *value)) {
    //! Untested
    //TODO Test properly
    assert(map != NULL);

    if (callback == NULL) return;

    for (size_t i = 0; i < map->socket_count; i++) {
        struct HashMapItem *item = map->sockets[i];

        while (item != NULL) {
            char *key = malloc(strlen(item->key)+1);
            strcpy(key, item->key);
            void *value = item->value;
            item = item->next;

            callback(key, value);

            free(key);
        }
    }
}

void hm_set_new_hash(struct HashMap *map, HashFunction hash) {
    assert(map != NULL);
    assert(hash != NULL);

}

void hm_print(struct HashMap *map, void print_value(void *value)) {
    assert(map != NULL);

    if (print_value == NULL) print_value = default_print_value;

    printf("====START====\n");
    for (size_t i = 0; i < map->socket_count; i++) {
        struct HashMapItem *item = map->sockets[i];
        if (item == NULL) continue;

        printf("Socket #%05lu:   ", i);

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
