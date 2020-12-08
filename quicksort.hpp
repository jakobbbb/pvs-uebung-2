#ifndef QUICKSORT_HPP

#define NUM 32767  // Elementanzahl

/** Vertausche zwei Zahlen im Feld v an der Position i und j */
void swap(float* v, int i, int j);

/** Serielle Version von Quicksort (Wirth) */
void quicksort(float* v, int start, int end);

/** Initialize vector with n random floats. */
float* init_vector(int n);

#define QUICKSORT_HPP
#endif  // QUICKSORT_HPP
