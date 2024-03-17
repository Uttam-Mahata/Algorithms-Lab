#include <stdio.h>
#include <stdlib.h>

// Function to merge two subarrays arr[l..m] and arr[m+1..r] into arr[]
void merge(double arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create dynamically allocated temporary arrays
    double *L = (double *)malloc(n1 * sizeof(double));
    double *R = (double *)malloc(n2 * sizeof(double));
    if (L == NULL || R == NULL) {
        printf("Error: Memory allocation failed\n");
        return;
    }

    // Copy data to temporary arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temporary arrays back into arr[l..r]
    i = 0;     // Initial index of first subarray
    j = 0;     // Initial index of second subarray
    k = l;     // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    // Free dynamically allocated memory
    free(L);
    free(R);
}

// Main function to implement Merge Sort
void mergeSort(double arr[], int l, int r) {
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        // Merge the sorted halves
        merge(arr, l, m, r);
    }
}

// Function to read data from a CSV file
int read_from_csv(const char *filename, double *data, int n) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return 0;
    }

    fclose(file);
    return 1;
}

// Function to check if an array is sorted
int is_sorted(double *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0; // Not sorted
        }
    }
    return 1; // Sorted
}

int main() {
    const char *filename = "normal_data_1.csv"; // Change to the filename you want to test

    int n = 1000000; // Change to the size of the dataset

    double *data = (double *)malloc(n * sizeof(double));

    // Read data from CSV file
    if (!read_from_csv(filename, data, n)) {
        free(data);
        return 1;
    }

    // Apply Merge Sort
    mergeSort(data, 0, n - 1);

    // Check if the array is sorted
    if (is_sorted(data, n)) {
        printf("Array is correctly sorted.\n");
    } else {
        printf("Array is not sorted.\n");
    }

    // Free allocated memory
    free(data);

    return 0;
}
