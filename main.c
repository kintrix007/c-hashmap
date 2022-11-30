#include <stdlib.h>
#include <stdio.h>

#include "hashmap.h"

int main() {
    struct HashMap *hm = hm_new(64);

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

    hm_print(hm, NULL);

    int a = *(int*)hm_get(hm, "Joe");
    printf("Joe: %d\n", a);
    a = *(int*)hm_get(hm, "Lisa");
    printf("Lisa: %d\n", a);
    a = *(int *)hm_get(hm, "Mark");
    printf("Mark: %d\n", a);
    a = *(int *)hm_get(hm, "Ur mom");
    printf("Ur mom: %d\n", a);
    printf("Lucas: %p\n", hm_get(hm, "Lucas"));

    hm_free(hm, free);
}
