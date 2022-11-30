#include <stdlib.h>
#include "../lib/gest.h"
#include "../lib/gest-run.h"

#include "../hashmap.h"

void test1() {
    struct HashMap *hm = hm_new(2);
    int *val;
    
    val = malloc(sizeof(int));
    *val = 1;
    hm_set(hm, "one", val);
    val = malloc(sizeof(int));
    *val = 2;
    hm_set(hm, "two", val);
    val = malloc(sizeof(int));
    *val = 3;
    hm_set(hm, "three", val);

    val = hm_get(hm, "two");
    assert_that(val != NULL);
    assert_int_equals(*(int*)val, 2);
    val = hm_get(hm, "one");
    assert_that(val != NULL);
    assert_int_equals(*(int*)val, 1);
    val = hm_get(hm, "four");
    assert_that(val == NULL);
    val = hm_get(hm, "three");
    assert_that(val != NULL);
    assert_int_equals(*(int*)val, 3);

    hm_free(hm, free);
}

void register_tests() {
    register_test(test1);
}
