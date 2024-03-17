#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 100000  // Size of the input array
#define NUM_BUCKETS 10   // Number of buckets

// Structure for a node in the linked list
struct Node {
    int data;
    struct Node* next;
};

// Function to insert a value into a linked list
void insert(struct Node** head, int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = NULL;
    
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    
    struct Node* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}

// Function to print a linked list
void printList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

// Function to perform insertion sort on a linked list
struct Node* insertionSort(struct Node* head) {
    struct Node* sorted = NULL;
    struct Node* current = head;
    while (current != NULL) {
        struct Node* next = current->next;
        struct Node* temp = sorted;
        if (sorted == NULL || temp->data >= current->data) {
            current->next = sorted;
            sorted = current;
        } else {
            while (temp->next != NULL && temp->next->data < current->data) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    return sorted;
}

// Function to perform Bucket Sort with linked list implementation
void bucketSort(int array[], int size) {
    struct Node* buckets[NUM_BUCKETS] = { NULL };

    // Insert elements into buckets
    for (int i = 0; i < size; ++i) {
        int index = array[i] / (ARRAY_SIZE / NUM_BUCKETS);
        insert(&buckets[index], array[i]);
    }

    // Sort individual buckets using insertion sort
    for (int i = 0; i < NUM_BUCKETS; ++i) {
        buckets[i] = insertionSort(buckets[i]);
    }

    // Concatenate sorted buckets
    int index = 0;
    for (int i = 0; i < NUM_BUCKETS; ++i) {
        struct Node* current = buckets[i];
        while (current != NULL) {
            array[index++] = current->data;
            current = current->next;
        }
    }
}

int main() {
    // Seed for random number generation
    srand(time(NULL));

    // Variables for time measurement
    clock_t start, end;
    double cpu_time_used;

    // Perform experiments for both uniform and normal data sets
    for (int i = 1; i <= 5; ++i) {
        // Read data from CSV files
        char filename[20];
        sprintf(filename, "normal_data_%d.csv", i);
        FILE* file = fopen(filename, "r");
        if (file == NULL) {
            printf("Error: Unable to open file %s\n", filename);
            return 1;
        }

        int data[ARRAY_SIZE];
        for (int j = 0; j < ARRAY_SIZE; ++j) {
            fscanf(file, "%d,", &data[j]);
        }
        fclose(file);

        // Measure time taken for sorting
        start = clock();
        bucketSort(data, ARRAY_SIZE);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

        printf("Normal Data Set %d - Time taken: %f seconds\n", i, cpu_time_used);
    }

    for (int i = 1; i <= 5; ++i) {
        // Read data from CSV files
        char filename[20];
        sprintf(filename, "uniform_data_%d.csv", i);
        FILE* file = fopen(filename, "r");
        if (file == NULL) {
            printf("Error: Unable to open file %s\n", filename);
            return 1;
        }

        int data[ARRAY_SIZE];
        for (int j = 0; j < ARRAY_SIZE; ++j) {
            fscanf(file, "%d,", &data[j]);
        }
        fclose(file);

        // Measure time taken for sorting
        start = clock();
        bucketSort(data, ARRAY_SIZE);
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

        printf("Uniform Data Set %d - Time taken: %f seconds\n", i, cpu_time_used);
    }

    return 0;
}
