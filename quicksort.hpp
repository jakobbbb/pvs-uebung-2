#ifndef QUICKSORT_HPP
#define QUICKSORT_HPP

#define NUM_ELEMENTS 32767  // Elementanzahl

/** Vertausche zwei Zahlen im Feld v an der Position i und j */
void swap(float* v, int i, int j);

/** Serielle Version von Quicksort (Wirth) */
void quicksort(float* v, int start, int end);

/** Parallel variants*/
void quicksort_a(float* v, int start, int end);
void quicksort_b(float* v, int start, int end);
void quicksort_c(float* v, int start, int end);

/** Initialize vector with n random floats. */
void fill_vector(float* v, int n);

/** Initialize vector with n random floats. */
float* init_vector(int n);

#endif  // QUICKSORT_HPP
