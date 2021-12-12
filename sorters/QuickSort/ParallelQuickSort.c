#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cilk/cilk.h>
#include "SequentialQuickSort.h"

#define sequentialThreshold 50

// int parallelPartition(int* input, int length){

// }

void parallelQuickSortKernel(int* input, int length){
    // sequential quicksort base case
    if (length <= sequentialThreshold){
        sequentialQuickSort(input, length);
        return;
    }
    // TODO: write and use parallelPartition instead of sequential
    int pivot = sequentialPartition(input, length);
    int leftLength = pivot;
    int* rightArray = input+pivot+1;
    int rightLength = length-leftLength-1;
    cilk_spawn parallelQuickSortKernel(input, leftLength);
    parallelQuickSortKernel(rightArray, rightLength);
}

void parallelQuickSort(int* input, int length){
    parallelQuickSortKernel(input, length);
    cilk_sync;
}