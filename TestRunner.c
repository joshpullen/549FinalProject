#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorters/SequentialMergeSort.c"
#include "sorters/ParallelMergeSort.c"


int main(){
    int n = 500000;
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
    if (correct == 1){
        printf("Correct.\n");
    }else{
        printf("Incorrect.\n");
    }


    return 0;
}