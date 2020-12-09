#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include "quicksort.hpp"

#define NUM_RUNS 500

typedef void (*quicksort_func)(float*, int, int);

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

double test_sort_once(quicksort_func sort, float* v, int num_elements) {
    fill_vector(v, num_elements);
    double start = omp_get_wtime();
    sort(v, 0, num_elements);
    double end = omp_get_wtime();
    assert(is_sorted(v, num_elements));
    return end - start;
}

double test_sort(quicksort_func sort, const char* name, double time_serial=0) {
    printf("Testing sorting function '%s'...\n", name);

    for (int n = 1; n < 10; ++n) {
        test_sort_once(sort, init_vector(n), n - 1);
    }
    double time_taken = 0;
    for (int i = 0; i < NUM_RUNS; ++i) {
        time_taken +=
            test_sort_once(sort, init_vector(NUM_ELEMENTS), NUM_ELEMENTS - 1);
    }
    printf("On average, it took %.3f ms to sort %d elements!\n",
           time_taken / NUM_RUNS * 1000, NUM_ELEMENTS);
    if (time_serial != 0) {
        double speedup = time_serial / time_taken;
        printf("Speedup is %.4f %s\n", speedup, speedup > 1 ? ":)" : ":(");
    }
    return time_taken;
}

int test() {
    printf("Testing...\n");
    test_is_sorted();

    double time_serial = test_sort(quicksort, "serial");
    // test_sort(quicksort_a, "parallel variant A", time_serial);
    // test_sort(quicksort_b, "parallel variant B", time_serial);
    test_sort(quicksort_c, "parallel variant C", time_serial);

    printf("All went well :)\n");
    return 0;
}
