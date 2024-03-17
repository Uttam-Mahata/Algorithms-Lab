#include "merge_sort.h"

void merge_sort(double arr[], int low, int high, MergeSortStats *stats) {
    if (low < high) {
        int mid = low + (high - low) / 2;
        merge_sort(arr, low, mid, stats);
        merge_sort(arr, mid + 1, high, stats);
        merge(arr, low, mid, high, stats);
    }
}

void merge(double arr[], int low, int mid, int high, MergeSortStats *stats) {
    int n1 = mid - low + 1;
    int n2 = high - mid;

    double L[n1], R[n2];
    for (int i = 0; i < n1; i++)
        L[i] = arr[low + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = low;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
        stats->comparisons++;
        stats->swaps++;
    }

    while (i < n1) {
        arr[k++] = L[i++];
        stats->swaps++;
    }

    while (j < n2) {
        arr[k++] = R[j++];
        stats->swaps++;
    }
}
