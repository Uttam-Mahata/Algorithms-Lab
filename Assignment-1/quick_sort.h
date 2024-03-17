#ifndef QUICK_SORT_H
#define QUICK_SORT_H

typedef struct {
    int comparisons;
    int swaps;
} QuickSortStats;

void quick_sort(double arr[], int low, int high, QuickSortStats *stats);

#endif

