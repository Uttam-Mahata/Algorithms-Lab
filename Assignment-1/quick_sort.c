#include "quick_sort.h"

void quick_sort(double arr[], int low, int high, QuickSortStats *stats) {
    if (low < high) {
        int pi = partition(arr, low, high, stats);
        quick_sort(arr, low, pi - 1, stats);
        quick_sort(arr, pi + 1, high, stats);
    }
}

int partition(double arr[], int low, int high, QuickSortStats *stats) {
    double pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            double temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            stats->swaps++;
        }
        stats->comparisons++;
    }
    double temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    stats->swaps++;
    return i + 1;
}
