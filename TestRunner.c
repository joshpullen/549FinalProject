#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cilk/cilk.h>
#include "sorters/SequentialMergeSort.c"
#include "sorters/ParallelMergeSort.c"
#include "sorters/KWayMergeSort/KWayMergeSort.c"
#include "sorters/KWayMergeSort/heap.c"
#include "sorters/QuickSort/SequentialQuickSort.c"
#include "sorters/QuickSort/ParallelQuickSort.c"
#include "sorters/COSort/SequentialCOSort.c"
#include "sorters/COSort/ParallelCOSort.c"
#include "sorters/QuickSort/ParallelQuickSort1.c"

// Check if arr is sorted
int check(int *arr, int n) {
    for (int i = 0; i < n - 1; ++ i) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

int checkArraysEqual(int *A, int *B, int n){
    for (int i = 0; i < n; i++){
        if (A[i]!=B[i]){
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Usage: %s <length of array>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    int *array;
    array = (int*)malloc(sizeof(int) * n);
    time_t t;
    srand((unsigned) time(&t));

    int i;
    for (i = 0; i < n; i++){
      array[i] = rand();
    }
    int *sorted;
    sorted = (int*)malloc(sizeof(int) * n);
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
    int *parsorted;
    parsorted = (int*)malloc(sizeof(int) * n);
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
    int *kWaySorted, k = 7;
    kWaySorted = (int*)malloc(sizeof(int) * n);
    tic = clock();
    kWayMergeSort(array, kWaySorted, k, n);
    toc = clock();
    printf("Time taken: %f seconds\n", (double)(toc-tic)/CLOCKS_PER_SEC);
    if (check(sorted, n) && check(parsorted, n) && check(kWaySorted, n)) printf("Correct\n");
    else printf("Error\n");



    printf("Sequential Quicksort:\n");
    int *sqSorted;
    sqSorted = (int*)malloc(sizeof(int) * n);
    memcpy(sqSorted, array, sizeof(int) * n);
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
    int *pqSorted;
    pqSorted = (int*)malloc(sizeof(int) * n);
    memcpy(pqSorted, array, sizeof(int) * n);
    tic = clock();
    parallelQuickSort(pqSorted, n);
    toc = clock();
    printf("Time taken: %f seconds\n", (double)(toc-tic)/CLOCKS_PER_SEC);
    if (check(pqSorted, n) == 1){
        printf("Correct.\n");
    }else{
        printf("Incorrect.\n");
    }

    printf("Parallel Quicksort1:\n");
    int *pqSorted1;
    pqSorted1 = (int*)malloc(sizeof(int) * n);
    memcpy(pqSorted1, array, sizeof(int) * n);
    tic = clock();
    parallelQuickSort1(pqSorted1, n);
    toc = clock();
    printf("Time taken: %f seconds\n", (double)(toc-tic)/CLOCKS_PER_SEC);
    if (check(pqSorted1, n) == 1){
        printf("Correct.\n");
    }else{
        printf("Incorrect.\n");
    }



    printf("Sequential COSort:\n");
    tic = clock();
    int* output = COSort(array, n);
    toc = clock();
    printf("Time taken: %f seconds\n", (double)(toc-tic)/CLOCKS_PER_SEC);
    if (check(output, n) == 1){
        printf("Correct.\n");
    }else{
        printf("Incorrect.\n");
    }
    printf("Contents check:\n");
    if (checkArraysEqual(output, pqSorted, n) == 1){
        printf("Correct contents.\n");
    }else{
        printf("Incorrect contents.\n");
    }
    

    printf("Parallel COSort:\n");
    tic = clock();
    int* parOutput = Parallel_COSort(array, n);
    toc = clock();
    printf("Time taken: %f seconds\n", (double)(toc-tic)/CLOCKS_PER_SEC);
    if (check(parOutput, n) == 1){
        printf("Correct.\n");
    }else{
        printf("Incorrect.\n");
    }
    printf("Contents check:\n");
    if (checkArraysEqual(parOutput, pqSorted, n) == 1){
        printf("Correct contents.\n");
    }else{
        printf("Incorrect contents.\n");
    }

    return 0;
}