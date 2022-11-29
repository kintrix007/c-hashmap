#include "hashmap.h"

int main() {
    struct HashMap *hm = hm_new(128);
    hm_free(hm);
}
