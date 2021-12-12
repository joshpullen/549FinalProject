#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorters/SequentialMergeSort.c"
#include "sorters/ParallelMergeSort.c"
#include "sorters/KWayMergeSort/KWayMergeSort.c"
#include "sorters/KWayMergeSort/heap.c"
#include "sorters/QuickSort/SequentialQuickSort.c"
#include "sorters/QuickSort/ParallelQuickSort.c"

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
    for (i = 0; i < n; i++){
      array[i] = rand();
    }
    int sorted[n];
    clock_t tic = clock();
    SequentialMergeSort(array, sorted, n);
    clock_t toc = clock();
    printf("Array size: %d\n", n);
    printf("Sorted Sequentially: \n");
    printf("Time taken: %f seconds\n", (double)(toc-tic)/CLOCKS_PER_SEC);
    if (check(sorted, n) == 1){
        printf("Correct.\n");
    }else{
        printf("Incorrect.\n");
    }


    printf("\nParallel: \n");
    int parsorted[n];
    tic = clock();
    ParallelMergeSort(array, parsorted, n);
    toc = clock();
    printf("Time taken: %f seconds\n", (double)(toc-tic)/CLOCKS_PER_SEC);
    if (check(parsorted, n) == 1){
        printf("Correct.\n");
    }else{
        printf("Incorrect.\n");
    }


    printf("\nMulti-way: \n");
    // k can be equals to [2..n]
    int kWaySorted[n], k = 7;
    tic = clock();
    kWayMergeSort(array, kWaySorted, k, n);
    toc = clock();
    printf("Time taken: %f seconds\n", (double)(toc-tic)/CLOCKS_PER_SEC);
    if (check(sorted, n) && check(parsorted, n) && check(kWaySorted, n)) printf("Correct\n");
    else printf("Error\n");


    printf("Sequential Quicksort:\n");
    int sqSorted[n];
    memcpy(sqSorted, array, sizeof(sqSorted));
    tic = clock();
    sequentialQuickSort(sqSorted, n);
    toc = clock();
    printf("Time taken: %f seconds\n", (double)(toc-tic)/CLOCKS_PER_SEC);
    if (check(sqSorted, n) == 1){
        printf("Correct.\n");
    }else{
        printf("Incorrect.\n");
    }


    printf("Parallel Quicksort:\n");
    int pqSorted[n];
    memcpy(pqSorted, array, sizeof(pqSorted));
    tic = clock();
    sequentialQuickSort(pqSorted, n);
    toc = clock();
    printf("Time taken: %f seconds\n", (double)(toc-tic)/CLOCKS_PER_SEC);
    if (check(pqSorted, n) == 1){
        printf("Correct.\n");
    }else{
        printf("Incorrect.\n");
    }
    printf("%d", pqSorted[100]);

    return 0;
}