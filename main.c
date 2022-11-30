#include <stdlib.h>
#include <stdio.h>

#include "hashmap.h"

int main() {
    struct HashMap *hm = hm_new(1);

    int *val = malloc(sizeof(int));
    *val = 5;
    hm_set(hm, "Joe", val);
    val = malloc(sizeof(int));
    *val = 10;
    hm_set(hm, "Lisa", val);
    val = malloc(sizeof(int));
    *val = 7;
    hm_set(hm, "Mark", val);
    hm_print(hm, NULL);
    val = malloc(sizeof(int));
    *val = 9999;
    hm_set(hm, "Ur mom", val);

    // hm_print(hm, NULL);

    // void *value = hm_get(hm, "Joe");
    // if (value != NULL) printf("Joe: %d\n", *(int*)value);
    // value = hm_get(hm, "Lucas");
    // if (value != NULL) printf("Lucas: %d\n", *(int *)value);

    // value = hm_remove(hm, "Joe");
    // if (value != NULL) free(value);
    // value = hm_remove(hm, "Robert");
    // if (value != NULL) free(value);

    // hm_print(hm, NULL);

    hm_free(hm, free);
}
