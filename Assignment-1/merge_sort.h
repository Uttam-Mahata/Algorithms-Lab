#ifndef MERGE_SORT_H
#define MERGE_SORT_H

typedef struct {
    int comparisons;
    int swaps;
} MergeSortStats;

void merge_sort(double arr[], int low, int high, MergeSortStats *stats);

#endif
