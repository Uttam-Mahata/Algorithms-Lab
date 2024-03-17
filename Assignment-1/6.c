#include <stdio.h>
#include <stdlib.h>

#define DATASET_SIZE 10000

typedef struct Node
{
    double data;
    struct Node *next;
} Node;

Node *Insertion_sort(Node *head){
    if (head == NULL || head->next == NULL)
        return head;
    Node *p = head;
    Node *q = head->next;
    q = Insertion_sort(q);
    if (p->data <= q->data){
        p->next = q;
        return p;}
    Node *start = q;
    while (q->next != NULL){
        if (p->data < q->next->data)
            break;
        else
            q = q->next;}
    p->next = q->next;
    q->next = p;
    return start;}

// Sorting function
void BucketSort(double arr[], int n)
{   int i, j;
    struct Node **buckets;
    // Create buckets and allocate memory size
    buckets = (struct Node **)malloc(sizeof(struct Node *) * n);
    // Initialize empty buckets
    for (i = 0; i < n; i++){
        buckets[i] = NULL;}
    // Fill the buckets with respective elements
    for (i = 0; i < n; ++i){
        struct Node *current;
        int pos = arr[i] * n;
        current = (struct Node *)malloc(sizeof(struct Node));
        current->data = arr[i];
        current->next = buckets[pos];
        buckets[pos] = current;}
    for (i = 0; i < n; ++i){
        buckets[i] = Insertion_sort(buckets[i]);}
    for (j = 0, i = 0; i < n; ++i){
        struct Node *node;
        node = buckets[i];
        while (node){
            arr[j++] = node->data;
            node = node->next;}}}
void save_to_file(const char *filename, double data[], int size) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < size; i++) {
    fprintf(file, "%.15lf\n", data[i]);
  }

  fclose(file);
}

int main() {
    FILE *file_uniform = fopen("normalized_uniform_data_1.txt", "r");
    FILE *file_normal = fopen("normalized_normal_data_1.txt", "r");

    double uniform_data[DATASET_SIZE];
    double normal_data[DATASET_SIZE];

    for (int i = 0; i < DATASET_SIZE; i++) {
        fscanf(file_uniform, "%lf\n", &uniform_data[i]);
        fscanf(file_normal, "%lf\n", &normal_data[i]);
    }

    fclose(file_uniform);
    fclose(file_normal);

    BucketSort(uniform_data,DATASET_SIZE);
    BucketSort(normal_data,DATASET_SIZE);

    for(int i=0;i<100;i++)
    {
        printf("%lf\n",uniform_data[i]);
    }
    printf("\n");
    for(int i=0;i<100;i++)
    {
        printf("%lf\n",normal_data[i]);
    }


    save_to_file("sorted_normalized_uniform_data.txt", uniform_data, DATASET_SIZE);
    save_to_file("sorted_normalized_normal_data.txt", normal_data, DATASET_SIZE);

    return 0;
}