#include <stdlib.h>

#include "hashmap.h"

int main() {
    struct HashMap *hm = hm_new(128);

    int *val = malloc(sizeof(int));
    *val = 5;
    hm_set(hm, "Joe", val);
    val = malloc(sizeof(int));
    *val = 10;
    hm_set(hm, "Lisa", val);

    hm_free(hm, free);
}
