#include <stdio.h>
#include <stdlib.h>
#include "sorters/SequentialMergeSort.c"
#include "sorters/ParallelMergeSort.c"


int main(){
    int n = 20;
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
    printf("\n");

    return 0;
}