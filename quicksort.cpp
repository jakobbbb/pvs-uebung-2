#include "quicksort.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.hpp"

/** Vertausche zwei Zahlen im Feld v an der Position i und j */
void swap(float* v, int i, int j) {
    float t = v[i];
    v[i] = v[j];
    v[j] = t;
}

/** Serielle Version von Quicksort (Wirth) */
void quicksort(float* v, int start, int end) {
    int i = start, j = end;
    float pivot;

    pivot = v[(start + end) / 2];  // mittleres Element
    do {
        while (v[i] < pivot)
            i++;
        while (pivot < v[j])
            j--;
        if (i <= j) {  // wenn sich beide Indizes nicht beruehren
            swap(v, i, j);
            i++;
            j--;
        }
    } while (i <= j);
    if (start < j)               // Teile und herrsche
        quicksort(v, start, j);  // Linkes Segment zerlegen
    if (i < end)
        quicksort(v, i, end);  // Rechtes Segment zerlegen
}


#define THRES_A 800

void quicksort_a(float *v, int start, int end) {
  int i = start, j = end;
  float pivot;

  pivot = v[(start + end) / 2];
  do {
      while (v[i] < pivot)
          i++;
      while (pivot < v[j])
          j--;
      if (i <= j) {
          swap(v, i, j);
          i++;
          j--;
      }
 } while (i <= j);

  #pragma omp parallel for
  for (int k = 0; k <= 1; ++k)
  {
      if (k == 0 && start < j) {
        if (j - start > THRES_A)
            quicksort_a(v, start, j);
        else
            quicksort(v, start, j);
      }

      if (k == 1 && i < end) {
        if (end - i > THRES_A)
            quicksort_a(v, i, end);
        else
            quicksort(v, i, end);
      }
  }
}

#define THRES_B 2000

void quicksort_b(float* v, int start, int end) {
    int i = start, j = end;
    float pivot;

    pivot = v[(start + end) / 2];  // mittleres Element
    do {
        while (v[i] < pivot)
            i++;
        while (pivot < v[j])
            j--;
        if (i <= j) {  // wenn sich beide Indizes nicht beruehren
            swap(v, i, j);
            i++;
            j--;
        }
    } while (i <= j);

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

#define THRES_C 100

void _quicksort_c(float* v, int start, int end);

void quicksort_c(float* v, int start, int end) {
#pragma omp parallel
#pragma omp single
    _quicksort_c(v, start, end);
}

void _quicksort_c(float* v, int start, int end) {
    int i = start, j = end;
    float pivot;

    pivot = v[(start + end) / 2];  // mittleres Element
    do {
        while (v[i] < pivot)
            i++;
        while (pivot < v[j])
            j--;
        if (i <= j) {  // wenn sich beide Indizes nicht beruehren
            swap(v, i, j);
            i++;
            j--;
        }
    } while (i <= j);

    if (start < j)  // Teile und herrsche
#pragma omp task if(j - start > THRES_C)
        _quicksort_c(v, start, j);  // Linkes Segment zerlegen
    if (i < end)
#pragma omp task if(end - i > THRES_C)
        _quicksort_c(v, i, end);  // Rechtes Segment zerlegen
}

float* init_vector(int n) {
    float* v = (float*)calloc(n, sizeof(float));  // Speicher reservieren
    return v;
}

void fill_vector(float* v, int n) {
    for (int j = 0; j < n; j++)  // Mit Zufallszahlen initialisieren
        v[j] = (float)rand();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {  // Benutzungshinweis
        printf("Vector sorting\nUsage: %s <NumIter>\n", argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "test") == 0) {
        return test();
    }

    int iter = atoi(argv[1]);              // Wiederholungen
    float* v = init_vector(NUM_ELEMENTS);  // Feld

    printf("Perform vector sorting %d times...\n", iter);
    for (int i = 0; i < iter; i++) {        // Wiederhole das Sortieren
        quicksort(v, 0, NUM_ELEMENTS - 1);  // Sortierung
    }
    printf("\nDone.\n");
    return 0;
}
