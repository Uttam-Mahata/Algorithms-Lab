#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define MAX_N 1000000 // Maximum number of elements in the array
#define MAX_PATH_LENGTH 256 // Maximum length of file path
#define NUM_DATASETS 5 // Number of datasets

// Global variables to count comparisons and swaps
unsigned long long ms_comparisons = 0;
unsigned long long ms_swaps = 0;
unsigned long long qs_comparisons = 0;
unsigned long long qs_swaps = 0;

// Function to swap two elements
void swap(double *a, double *b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

// Function to merge two subarrays arr[l..m] and arr[m+1..r] into arr[]
void merge(double arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temporary arrays
    double L[n1], R[n2];

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
        ms_comparisons++; // Increment comparison count
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
        ms_swaps++; // Increment swap count
    }

    // Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
        ms_swaps++; // Increment swap count
    }
}

// Function to implement Merge Sort
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

// Function to partition the array and return the pivot index
int partition(double arr[], int low, int high) {
    double pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        qs_comparisons++; // Increment comparison count
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
            qs_swaps++; // Increment swap count
        }
    }
    swap(&arr[i + 1], &arr[high]);
    qs_swaps++; // Increment swap count
    return i + 1;
}

// Function to implement Quick Sort
void quickSort(double arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Function to read data from a CSV file
int read_from_csv(const char *filename, double *data, int n) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return 0;
    }

    for (int i = 0; i < n; i++) {
        if (fscanf(file, "%lf\n", &data[i]) != 1) {
            printf("Error: Failed to read from file %s\n", filename);
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}

// Function to save data to a CSV file
void save_to_csv(const char *filename, unsigned long long comparisons, unsigned long long swaps, double time_taken) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    fprintf(file, "Problem Size,Comparisons,Swaps,Time Taken (s)\n");
    fprintf(file, "%d,%llu,%llu,%.6f\n", MAX_N, comparisons, swaps, time_taken);

    fclose(file);
}

// Function to create a directory
int create_directory(const char *path) {
    #if defined(_WIN32)
        return _mkdir(path);
    #else
        return mkdir(path, 0777);
    #endif
}

int main() {
    // Seed the random number generator
    srand(time(NULL));

    // Create directories for saving results
    const char *ms_dir_name = "swaps_comparisons_ms";
    const char *qs_dir_name = "swaps_comparisons_qs";
    if (create_directory(ms_dir_name) != 0) {
        if (errno != EEXIST) {
            printf("Error: Unable to create directory %s\n", ms_dir_name);
            return 1;
        }
    }
    if (create_directory(qs_dir_name) != 0) {
        if (errno != EEXIST) {
            printf("Error: Unable to create directory %s\n", qs_dir_name);
            return 1;
        }
    }

    // Arrays to store data for Merge Sort and Quick Sort
    double *ms_data = (double *)malloc(MAX_N * sizeof(double));
    double *qs_data = (double *)malloc(MAX_N * sizeof(double));

    // Read and sort datasets for each problem size
    for (int problem_size = 1; problem_size <= MAX_N; problem_size *= 2) {
        // Generate and save datasets for each problem size
        for (int dataset = 1; dataset <= NUM_DATASETS; dataset++) {
            char filename[MAX_PATH_LENGTH];
            sprintf(filename, "uniform_data_%d.csv", dataset); // Change to the filename of the dataset
            printf("Reading dataset: %s\n", filename);

            // Read data from CSV file
            if (!read_from_csv(filename, ms_data, problem_size)) {
                free(ms_data);
                free(qs_data);
                return 1;
            }

            // Copy data for Quick Sort
            memcpy(qs_data, ms_data, problem_size * sizeof(double));

            // Merge Sort
            ms_comparisons = 0;
            ms_swaps = 0;
            clock_t start_ms = clock();
            mergeSort(ms_data, 0, problem_size - 1);
            clock_t end_ms = clock();
            double time_taken_ms = ((double)(end_ms - start_ms)) / CLOCKS_PER_SEC;

            // Save results for Merge Sort
            char ms_filename[MAX_PATH_LENGTH];
            sprintf(ms_filename, "%s/uniform_ms_data_%d_%d.csv", ms_dir_name, problem_size, dataset);
            save_to_csv(ms_filename, ms_comparisons, ms_swaps, time_taken_ms);

            // Quick Sort
            qs_comparisons = 0;
            qs_swaps = 0;
            clock_t start_qs = clock();
            quickSort(qs_data, 0, problem_size - 1);
            clock_t end_qs = clock();
            double time_taken_qs = ((double)(end_qs - start_qs)) / CLOCKS_PER_SEC;

            // Save results for Quick Sort
            char qs_filename[MAX_PATH_LENGTH];
            sprintf(qs_filename, "%s/uniform_qs_data_%d_%d.csv", qs_dir_name, problem_size, dataset);
            save_to_csv(qs_filename, qs_comparisons, qs_swaps, time_taken_qs);

            // Print results for each dataset
            printf("Problem Size: %d, Dataset: %d\n", problem_size, dataset);
            printf("  Merge Sort: Comparisons = %llu, Swaps = %llu, Time Taken (s) = %.6f\n", ms_comparisons, ms_swaps, time_taken_ms);
            printf("  Quick Sort: Comparisons = %llu, Swaps = %llu, Time Taken (s) = %.6f\n", qs_comparisons, qs_swaps, time_taken_qs);
        }
    }

    // Free allocated memory
    free(ms_data);
    free(qs_data);

    return 0;
}
