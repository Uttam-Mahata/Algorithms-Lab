#include <stdio.h>
#include <stdlib.h>

// Define a structure for linked list node
struct Node {
    float data;
    struct Node* next;
};

// Function to create a new node
struct Node* createNode(float value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a node into the linked list
void insertNode(struct Node** head, float value) {
    struct Node* newNode = createNode(value);
    if (*head == NULL) {
        *head = newNode;
    } else {
        struct Node* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Function to print the linked list
void printList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("%.2f -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

// Function to concatenate two linked lists
struct Node* concatenateLists(struct Node* list1, struct Node* list2) {
    if (list1 == NULL) {
        return list2;
    }
    struct Node* current = list1;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = list2;
    return list1;
}

// Function to sort a linked list using insertion sort
struct Node* insertionSort(struct Node* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }
    struct Node* sorted = NULL;
    struct Node* current = head;
    while (current != NULL) {
        struct Node* nextNode = current->next;
        if (sorted == NULL || sorted->data >= current->data) {
            current->next = sorted;
            sorted = current;
        } else {
            struct Node* temp = sorted;
            while (temp->next != NULL && temp->next->data < current->data) {
                temp = temp->next;
            }
            if (temp->next != NULL) {
                current->next = temp->next;
            }
            temp->next = current;
        }
        current = nextNode;
    }
    return sorted;
}

// Function to apply bucket sort on a linked list
struct Node* bucketSort(struct Node* head, int numBuckets) {
    if (head == NULL || head->next == NULL || numBuckets <= 0) {
        return head;
    }
    
    // Create an array of linked lists as buckets
    struct Node* buckets[numBuckets];
    for (int i = 0; i < numBuckets; i++) {
        buckets[i] = NULL;
    }
    
    // Distribute the elements into buckets
    struct Node* current = head;
    while (current != NULL) {
        int index = current->data * numBuckets;
        insertNode(&buckets[index], current->data);
        current = current->next;
    }
    
    // Sort each bucket and concatenate them
    struct Node* sorted = NULL;
    for (int i = 0; i < numBuckets; i++) {
        buckets[i] = insertionSort(buckets[i]);
        sorted = concatenateLists(sorted, buckets[i]);
    }
    
    return sorted;
}

// Function to free the memory allocated for the linked list
void freeList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    // Read data from the CSV file and create the linked list
    FILE* file = fopen("uniform_data_1.csv", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    struct Node* head = NULL;
    float value;
    while (fscanf(file, "%f", &value) == 1) {
        insertNode(&head, value);
    }
    fclose(file);
    
    // Perform bucket sort
    int numBuckets = 10; // You can adjust the number of buckets as needed
    head = bucketSort(head, numBuckets);
    
    // Print the sorted list
    printf("Sorted list:\n");
    printList(head);
    
    // Free the memory allocated for the linked list
    freeList(head);
    
    return 0;
}
