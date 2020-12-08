#include <assert.h>
#include <stdio.h>
#include "quicksort.hpp"

bool is_sorted(float* v, int num) {
    for (int i = 1; i < num; ++i) {
        if (v[i] < v[i - 1]) {
            return false;
        }
    }
    return true;
}

void test_is_sorted() {
    printf("Testing is_sorted...\n");

    float v1[] = {1, 2, 3, 5, 8};
    assert(is_sorted(v1, 5));

    v1[2] = 512;
    assert(!is_sorted(v1, 5));

    float v2[] = {0};
    assert(is_sorted(v2, 1));

    float v3[] = {1, 2};
    assert(is_sorted(v3, 2));

    float v4[] = {2, 1};
    assert(!is_sorted(v4, 2));
}

int test() {
    printf("Testing...\n");
    test_is_sorted();
    return 0;
}
