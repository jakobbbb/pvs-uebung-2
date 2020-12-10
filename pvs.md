---
titlepage: true
title: "Parallele und Verteilte Systeme: WS20/21 -- Übung 2 -- Gruppe B"
author: Lisa Piekarski, Klara Wichmann, Jakob Béla Ruckel
date: "2020-12-10"
listings-no-page-break: true
...

# Overview of Speedups

| CPU / Cores / Threads | Speedup A | Speedup B | Speedup C |
|-----------------------|-----------|-----------|-----------|
| i5-3570K / 4 / 4      |           |           |    2.9375 |
| i5-3320M / 2 / 4      |    1.1131 |           |    2.0371 |
| i7-4702MQ / 4 / 8     |    1.1655 |    1.1345 |    3.6758 |


# Variant A

(TODO)

# Variant B

The second variation of a parallelized quicksort uses parallel sections. We devided the decomposition of the left and right segments in two different sections:

```#define THRES_B 2000

void quicksort_b(float* v, int start, int end) {
    /* ... */

#pragma omp parallel if (end-start > THRES_B)
    {
#pragma omp sections
        {
#pragma omp section
            {
                if (start < j)               // Teile und herrsche
                    quicksort(v, start, j);  // Linkes Segment zerlegen
            }
#pragma omp section
            {
                if (i < end)
                    quicksort(v, i, end);  // Rechtes Segment zerlegen
            }
        }
    }
}
```

We can do this at these positions because we have two code blocks which are independant of one another. Each section is now on a different thread.

To find a sweet spot for the threshold `THRES`, some tests were
performed (machine with 4 core and 8 threads):

| `THRES` | Speedup |
|---------|---------|
|       0 |  1.1281 |
|      10 |  1.1936 |
|     100 |  1.1707 |
|   1,000 |  1.1683 |
|   2,000 |  1.0966 |
|   5,000 |  1.1033 |
|  10,000 |  1.1304 |
| 100,000 |  0.9994 |

The sweet spot seems to be around 2000. For lists smaller than that, the
overhead of section creation overpowers the benefits of parallelization. 
The opposite applies to list larger than the threshold.

# Variant C

Our third variation on parallelized Quicksort attempts to reduce
overhead by eliminating the `parallel` and `single` `pragmas` from the
recursively called function.  This way, there is only a single place
where setting up of the threads occurs.

```cpp
void quicksort_c(float* v, int start, int end) {
#pragma omp parallel
#pragma omp single
    _quicksort_c(v, start, end);
}
```

In the `_quicksort_c` function, tasks are created to perform sorting on
the two halves:

```cpp
void _quicksort_c(float* v, int start, int end) {

    /* ... */

    if (start < j)  // Teile und herrsche
#pragma omp task if (j - start > THRES)
        _quicksort_c(v, start, j);  // Linkes Segment zerlegen
    if (i < end)
#pragma omp task if (end - i > THRES)
        _quicksort_c(v, i, end);  // Rechtes Segment zerlegen
}
```

To find a sweet spot for the threshold `THRES`, some tests were
performed (machine with 4 core and 4 threads):

| `THRES` | Speedup |
|---------|---------|
|       0 |  1.4075 |
|      10 |  2.0915 |
|      50 |  2.7537 |
|     100 |  2.9375 |
|     200 |  2.8739 |
|   1,000 |  2.7711 |
|  10,000 |  1.3835 |
| 100,000 |  0.8468 |

Note that the speed for a threshold of zero is what we'd have gotten
without the conditional `if` in the task `pragma`s.
As 100,000 is larger than the test list's size, we're seeing a slowdown
due to overhead.

The sweet spot seems to be around 100.  For lists smaller than that, the
overhead of task creation and scheduling overpowers the benefits of
parallelization. The opposite applies to list larger than the
threshold.
