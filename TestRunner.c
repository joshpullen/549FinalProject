#include <stdio.h>
#include <stdlib.h>
#include "sorters/SequentialMergeSort.c"
#include "sorters/ParallelMergeSort.c"
#include "sorters/KWayMergeSort/KWayMergeSort.c"
#include "sorters/KWayMergeSort/heap.c"

// Check if arr is sorted
int check(int *arr, int n) {
    int i = 0;
    for (int i = 0; i < n - 1; ++ i) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

int main(){
    int n = 10000;
    int array[n];
    time_t t;
    srand((unsigned) time(&t));

    int i;
    printf("Original:\n");
    for (i = 0; i < n; i++){
      array[i] = rand();
      printf("%d ", array[i]);
    }
    printf("\nSorted: \n");
    int sorted[n];
    SequentialMergeSort(array, sorted, n);
    for (i = 0; i < n; i++){
        printf("%d ", sorted[i]);
    }
    printf("\nParallel: \n");
    int parsorted[n];
    ParallelMergeSort(array, parsorted, n);
    for (i = 0; i < n; i++){
        printf("%d ", parsorted[i]);
    }
    printf("\nMulti-way: \n");
    // k can be equals to [2..n]
    int kWaySorted[n], k = 7;
    kWayMergeSort(array, kWaySorted, k, n);
    for (i = 0; i < n; i++){
        printf("%d ", kWaySorted[i]);
    }
    printf("\n");
    if (check(sorted, n) && check(parsorted, n) && check(kWaySorted, n)) printf("Correct\n");
    else printf("Error\n");
    return 0;
}