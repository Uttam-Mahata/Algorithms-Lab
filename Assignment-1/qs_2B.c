#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATASET_SIZE 100000

long long quick_sort_comparisons = 0;
long long quick_sort_swaps = 0;

int partition(double arr[], int low, int high) {
  double pivot = arr[high];
  int i = low - 1;

  for (int j = low; j <= high - 1; j++) {
    quick_sort_comparisons++;
    if (arr[j] <= pivot) {
      i++;
      double temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
      quick_sort_swaps++;
    }
  }

  double temp = arr[i + 1];
  arr[i + 1] = arr[high];
  arr[high] = temp;
  quick_sort_swaps++;

  return i + 1;
}

void quick_sort(double arr[], int low, int high) {
  if (low < high) {
    int pi = partition(arr, low, high);

    quick_sort(arr, low, pi - 1);
    quick_sort(arr, pi + 1, high);
  }
}

void save_to_csv(const char *filename, int problem_size, long long comparisons,
                 long long swaps, double time_taken) {
  FILE *file = fopen(filename, "a");

  if (file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  fprintf(file, "%d,%lld,%lld,%.6f\n", problem_size, comparisons, swaps,
          time_taken);

  fclose(file);
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
  srand(time(NULL));

  FILE *quick_sort_uniform_csv = fopen("quick_sort_uniform_data.csv", "w");
  FILE *quick_sort_normal_csv = fopen("quick_sort_normal_data.csv", "w");

  if (quick_sort_uniform_csv == NULL || quick_sort_normal_csv == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  fprintf(quick_sort_uniform_csv,
          "Problem Size,QS Comparisons,QS Swaps,Time Taken (s)\n");
  fprintf(quick_sort_normal_csv,
          "Problem Size,QS Comparisons,QS Swaps,Time Taken (s)\n");

  printf("| Problem Size | QS Comparisons | QS Swaps | Time Taken (s) |\n");
  printf("|--------------|----------------|----------|----------------|\n");

  for (int i = 1; i <= DATASET_SIZE; i *= 2) {
    clock_t start, end;
    double cpu_time_used;

    FILE *file_uniform = fopen("uniform_data.csv", "r");
    FILE *file_normal = fopen("normal_data.csv", "r");

    double uniform_data[i];
    double normal_data[i];

    for (int j = 0; j < i; j++) {
      fscanf(file_uniform, "%lf\n", &uniform_data[j]);
      fscanf(file_normal, "%lf\n", &normal_data[j]);
    }

    fclose(file_uniform);
    fclose(file_normal);

    quick_sort_comparisons = 0;
    quick_sort_swaps = 0;

    start = clock();

    quick_sort(uniform_data, 0, i - 1);

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("| %-13d | %-14lld | %-8lld | %-14.6f |\n", i,
           quick_sort_comparisons, quick_sort_swaps, cpu_time_used);

    fprintf(quick_sort_uniform_csv, "%d,%lld,%lld,%.6f\n", i,
            quick_sort_comparisons, quick_sort_swaps, cpu_time_used);

    start = clock();

    quick_sort(normal_data, 0, i - 1);

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    fprintf(quick_sort_normal_csv, "%d,%lld,%lld,%.6f\n", i,
            quick_sort_comparisons, quick_sort_swaps, cpu_time_used);
  }


  fclose(quick_sort_uniform_csv);
  fclose(quick_sort_normal_csv);

  return 0;
}
