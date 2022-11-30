#include <stdlib.h>
#include <stdio.h>

#include "hashmap.h"

static void my_print_thingy(char *key, void *value);

int main() {
    struct HashMap *hm = hm_new(4, free);

    int *val = malloc(sizeof(int));
    *val = 5;
    hm_set(hm, "Joe", val);
    val = malloc(sizeof(int));
    *val = 10;
    hm_set(hm, "Lisa", val);
    val = malloc(sizeof(int));
    *val = 7;
    hm_set(hm, "Mark", val);
    val = malloc(sizeof(int));
    *val = 9999;
    hm_set(hm, "Ur mom", val);

    printf("\n");

    hm_print(hm, NULL);

    printf("\n");

    void *value = hm_get(hm, "Joe");
    if (value != NULL) printf("-> Joe: %d\n", *(int*)value);
    else printf("-> Joe: (nil)\n");

    value = hm_get(hm, "Lucas");
    if (value != NULL) printf("-> Lucas: %d\n", *(int*)value);
    else printf("-> Lucas: (nil)\n");

    printf("\n");

    hm_foreach(hm, my_print_thingy);

    printf("\n");

    value = hm_remove(hm, "Joe");
    if (value != NULL) free(value);
    value = hm_remove(hm, "Robert");
    if (value != NULL) free(value);

    hm_print(hm, NULL);

    hm_free(hm);
}

static void my_print_thingy(char *key, void* value) {
    printf("Foreach => ");
    if (value == NULL) {
        printf("%s : %p\n", key, value);
    } else {
        printf("%s : %d\n", key, *(int*)value);
    }
}
