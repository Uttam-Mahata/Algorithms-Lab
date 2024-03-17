#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define N 100000 // Number of random numbers to generate

// Function to generate random numbers from a uniform distribution
void generate_uniform(double *data, int n) {
    for (int i = 0; i < n; i++) {
        data[i] = (double)rand() / RAND_MAX * 10; // Generate a random number between 0 and 10
    }
}

// Function to generate random numbers from a normal distribution with given mean and standard deviation
void generate_normal(double *data, int n, double mean, double stddev) {
    for (int i = 0; i < n; i++) {
        double u1 = (double)rand() / RAND_MAX;
        double u2 = (double)rand() / RAND_MAX;
        #include <math.h>

        double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2); // Box-Muller transform
        data[i] = mean + stddev * z;
    }
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

int main() {
    // Seed the random number generator
    srand(time(NULL));

    // Generate and save datasets with values greater than 1 for uniform distribution
    for (int i = 1; i <= 5; i++) {
        // Allocate memory for the dataset
        double *uniform_data = (double *)malloc(N * sizeof(double));

        // Generate data from uniform distribution
        generate_uniform(uniform_data, N);

        // Save data to CSV file
        char uniform_filename[20];
        sprintf(uniform_filename, "uniform_data_%d.csv", i);
        save_to_csv(uniform_filename, uniform_data, N);

        printf("Uniform Dataset %d: Data saved to CSV file: %s\n", i, uniform_filename);

        // Free allocated memory
        free(uniform_data);
    }

    // Generate and save datasets with mean and standard deviation for normal distribution
    double mean = 5.0; // Mean
    double stddev = 4.0; // Standard deviation
    for (int i = 1; i <= 5; i++) {
        // Allocate memory for the dataset
        double *normal_data = (double *)malloc(N * sizeof(double));

        // Generate data from normal distribution
        generate_normal(normal_data, N, mean, stddev);

        // Save data to CSV file
        char normal_filename[20];
        sprintf(normal_filename, "normal_data_%d.csv", i);
        save_to_csv(normal_filename, normal_data, N);

        printf("Normal Dataset %d: Data saved to CSV file: %s\n", i, normal_filename);

        // Free allocated memory
        free(normal_data);
    }

    return 0;
}
