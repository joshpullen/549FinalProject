#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
<<<<<<< HEAD
    int n = 500000;
=======
    int n = 10000;
>>>>>>> 4a86c4d432f47acd61e35d42342558da5cea9ca2
    int array[n];
    time_t t;
    srand((unsigned) time(&t));

    int i;
    int sorted[n];
    clock_t tic = clock();
    SequentialMergeSort(array, sorted, n);
    clock_t toc = clock();
    printf("Array size: %d\n", n);
    printf("Sorted Sequentially: \n");
    printf("Time taken: %f seconds\n", (double)(toc-tic)/CLOCKS_PER_SEC);
    int correct = 1;
    for (i = 1; i < n; i++){
        if (sorted[i] < sorted[i-1]){
            correct = 0;
            break;
        }
    }
    if (correct == 1){
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
    correct = 1;
    for (i = 1; i < n; i++){
        if (parsorted[i] < parsorted[i-1]){
            correct = 0;
            break;
        }
    }
<<<<<<< HEAD
    if (correct == 1){
        printf("Correct.\n");
    }else{
        printf("Incorrect.\n");
    }


=======
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
>>>>>>> 4a86c4d432f47acd61e35d42342558da5cea9ca2
    return 0;
}