//  Implement the worst case linear median selection algorithm by taking the median of medians (MoM) as the pivotal element and check for correctness.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
  int n, i, j, k, l, m, r, temp, median, median_of_median;
  printf("Enter the number of elements: ");
  scanf("%d", &n);
  int a[n];
  // Take random uniform elements
  srand(time(NULL));
  for (i = 0; i < n; i++) {
    a[i] = rand() % 100;
    
  }

//   // Print the array
//   printf("The array is: ");
//   for (i = 0; i < n; i++) {
//     printf("%d ", a[i]);
    
//   }
  printf("\n");
  // Sort the array
  for (i = 0; i < n - 1; i++) {
    for (j = 0; j < n - i - 1; j++) {
      if (a[j] > a[j + 1]) {
        temp = a[j];
        a[j] = a[j + 1];
        a[j + 1] = temp;
        
      }
      
    }
    
  }
  // Print the sorted array
//   printf("The sorted array is: ");
//   for (i = 0; i < n; i++) {
//     printf("%d ", a[i]);
//   }
  printf("\n");
  // Find the median
  if (n % 2 == 0) {
    median = (a[n / 2] + a[n / 2 - 1]) / 2; 

  } else {
    median = a[n / 2];
  }
    printf("The median is: %d\n", median);
    // Find the median of medians
    int b[n / 5];
    for (i = 0; i < n / 5; i++) {
      for (j = 0; j < 5; j++) {
        b[i] = a[i * 5 + j];
      }
      for (k = 0; k < n / 5 - 1; k++) {
        for (l = 0; l < n / 5 - k - 1; l++) {
          if (b[l] > b[l + 1]) {
            temp = b[l];
            b[l] = b[l + 1];
            b[l + 1] = temp;
          }
        }
      }
    }
    if (n % 5 != 0) {
      for (m = 0; m < n % 5; m++) {
        b[n / 5] = a[n - n % 5 + m];
      }
      for (k = 0; k < n / 5; k++) {
        for (l = 0; l < n / 5 - k - 1; l++) {
          if (b[l] > b[l + 1]) {
            temp = b[l];
            b[l] = b[l + 1];
            b[l + 1] = temp;
          }
        }
      }
    }
    if ((n / 5) % 2 == 0) {
      median_of_median = (b[n / 10] + b[n / 10 - 1]) / 2;
    } else {
      median_of_median = b[n / 10];
    }
    printf("The median of medians is: %d\n", median_of_median);
    return 0;
  

  

    
}