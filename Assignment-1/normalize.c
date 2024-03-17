#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define N 100000 // Number of random numbers to generate
#define MAX_PATH_LENGTH 256 // Maximum length of file path

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
void save_to_csv(const char *filename, double *data, int n) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    for (int i = 0; i < n; i++) {
        fprintf(file, "%.6f\n", data[i]);
    }

    fclose(file);
}

// Function to normalize data between 0 and 1
void normalize_data(double *data, int n) {
    double min_val = data[0];
    double max_val = data[0];

    // Find minimum and maximum values
    for (int i = 1; i < n; i++) {
        if (data[i] < min_val) {
            min_val = data[i];
        }
        if (data[i] > max_val) {
            max_val = data[i];
        }
    }

    // Normalize data
    double range = max_val - min_val;
    for (int i = 0; i < n; i++) {
        data[i] = (data[i] - min_val) / range;
    }
}

// Function to create a directory
int create_directory(const char *path) {
    #if defined(_WIN32)
        return mkdir(path);
    #else
        return mkdir(path, 0777);
    #endif
}

int main() {
    // Create a directory for normalized data
    const char *dir_name = "normalized_data";
    if (create_directory(dir_name) != 0) {
        if (errno != EEXIST) {
            printf("Error: Unable to create directory %s\n", dir_name);
            return 1;
        }
    }

    // Normalize and save the datasets
    for (int i = 1; i <= 5; i++) {
        // Read data from CSV files
        char uniform_filename[MAX_PATH_LENGTH];
        char normal_filename[MAX_PATH_LENGTH];
        sprintf(uniform_filename, "uniform_data_%d.csv", i);
        sprintf(normal_filename, "normal_data_%d.csv", i);
        double *uniform_data = (double *)malloc(N * sizeof(double));
        double *normal_data = (double *)malloc(N * sizeof(double));
        if (!read_from_csv(uniform_filename, uniform_data, N) || !read_from_csv(normal_filename, normal_data, N)) {
            // Error occurred while reading data
            free(uniform_data);
            free(normal_data);
            continue;
        }

        // Normalize data
        normalize_data(uniform_data, N);
        normalize_data(normal_data, N);

        // Save normalized data to CSV files in the "normalized_data" folder
        char normalized_uniform_filename[MAX_PATH_LENGTH];
        char normalized_normal_filename[MAX_PATH_LENGTH];
        sprintf(normalized_uniform_filename, "%s/uniform_data_%d.csv", dir_name, i);
        sprintf(normalized_normal_filename, "%s/normal_data_%d.csv", dir_name, i);
        save_to_csv(normalized_uniform_filename, uniform_data, N);
        save_to_csv(normalized_normal_filename, normal_data, N);

        printf("Dataset %d: Normalized data saved to CSV files: %s and %s\n", i, normalized_uniform_filename, normalized_normal_filename);

        // Free allocated memory
        free(uniform_data);
        free(normal_data);
    }

    return 0;
}
