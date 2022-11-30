#include <string.h>
#include <stdlib.h>
#include "../lib/gest.h"
#include "../lib/gest-run.h"

#include "../hashmap.h"

void test_hm_free() {
    struct HashMap *hm = hm_new(150, free);
    char *key;

    for (char i = '!'; i < '~'; i++) {
        key = malloc(sizeof(char)*4);
        key[0] = i;
        key[1] = i;
        key[2] = i;
        key[3] = '\0';

        if (i % 11 == 0) {
            hm_set(hm, key, NULL);
        } else {
            char *val = malloc(sizeof(char) * 4);
            strcpy(val, "idk");
            hm_set(hm, key, val);
        }

        if (key != NULL) free(key);
    }

    // hm_print(hm, NULL);

    hm_free(hm);
}

void test_hm_set_get() {
    struct HashMap *hm = hm_new(2, free);
    int *val;

    val = malloc(sizeof(int));
    *val = 999;
    hm_set(hm, "one", val);

    val = malloc(sizeof(int));
    *val = 1;
    hm_set(hm, "one", val);     // Overrides (and frees) the previous "one" : 999

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

    hm_free(hm);
}

void test_hm_remove() {
    struct HashMap *hm = hm_new(5, free);
    double *val;

    val = malloc(sizeof(double));
    *val = 3.14159;
    hm_set(hm, "PI", val);
    val = malloc(sizeof(double));
    *val = 6.28318;
    hm_set(hm, "TAU", val);
    val = malloc(sizeof(double));
    *val = 2.718;
    hm_set(hm, "e", val);

    val = hm_get(hm, "TAU");
    assert_that(val != NULL);
    assert_dbl_equals(*(double*)val, 6.28318);

    val = hm_remove(hm, "TAU");
    assert_that(val != NULL);
    assert_dbl_equals(*(double*)val, 6.28318);
    free(val); //! Do NOT forget this

    val = hm_get(hm, "TAU");
    assert_that(val == NULL);

    val = hm_remove(hm, "TAU");
    assert_that(val == NULL);

    val = hm_get(hm, "PI");
    assert_that(val != NULL);
    assert_dbl_equals(*(double *)val, 3.14159);

    val = hm_get(hm, "e");
    assert_that(val != NULL);
    assert_dbl_equals(*(double *)val, 2.718);

    hm_free(hm);
}

void register_tests() {
    register_test(test_hm_free);
    register_test(test_hm_set_get);
    register_test(test_hm_remove);
}
