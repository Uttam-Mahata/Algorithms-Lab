#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10000

int A[N];

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int A[], int l, int r, int x) {
    int i;
    for (i = l; i < r; i++) {
        if (A[i] == x)
            break;
    }
    swap(&A[i], &A[r]);
    i = l;
    for (int j = l; j < r; j++) {
        if (A[j] <= x) {
            swap(&A[i], &A[j]);
            i++;
        }
    }
    swap(&A[i], &A[r]);
    return i;
}

int medianOfMedians(int A[], int l, int r, int k);

int select(int A[], int l, int r, int k, int partitionSize) {
    if (r - l < partitionSize) {
        for (int i = l + 1; i <= r; i++) {
            int j = i;
            while (j > l && A[j - 1] > A[j]) {
                swap(&A[j], &A[j - 1]);
                j--;
            }
        }
        return A[l + k - 1];
    }
    int mom = medianOfMedians(A, l, r, (r - l + 1) / 2);
    int i = partition(A, l, r, mom);
    int j = i - l + 1;
    if (k == j)
        return A[i];
    else if (k < j)
        return select(A, l, i - 1, k, partitionSize);
    else
        return select(A, i + 1, r, k - j, partitionSize);
}

int medianOfMedians(int A[], int l, int r, int k) {
    if (r - l < 5) {
        for (int i = l + 1; i <= r; i++) {
            int j = i;
            while (j > l && A[j - 1] > A[j]) {
                swap(&A[j], &A[j - 1]);
                j--;
            }
        }
        return A[l + (r - l) / 2];
    }
    for (int i = l; i <= r; i += 5) {
        int sub_r = i + 4;
        if (sub_r > r)
            sub_r = r;
        int median = select(A, i, sub_r, (sub_r - i + 1) / 2, 5); // Change the partition size here
        swap(&A[median], &A[l + (i - l) / 5]);
    }
    return select(A, l, l + (r - l) / 5, (r - l) / 10 + 1, 5); // Change the partition size here
}

int main() {
    srand(time(NULL));
//    take uniform_dataset.csv
    FILE *f = fopen("uniform_dataset_1.csv", "r");
    for (int i = 0; i < N; i++) {
        fscanf(f, "%d", &A[i]);
    }
    fclose(f);


    clock_t start, end;
    double cpu_time_used;

    printf("Partition Size\tTime (seconds)\n");

    for (int partitionSize = 3; partitionSize <= 201; partitionSize += 2) { // Try different partition sizes
        start = clock();
        int median = select(A, 0, N - 1, N / 2, partitionSize);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("%d\t\t%f\n", partitionSize, cpu_time_used);
    }
    
    return 0;
}
