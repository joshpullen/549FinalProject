#include "SequentialMergeSort.h"

void SequentialMergeSort(int* input, int* output, int min, int maxExclusive){
    if (maxExclusive-min <= 1){
        return;
    }
    // can't overflow, in case we are using really big arrays
    int mid = min+(maxExclusive-min)/2;
    SequentialMergeSort(input, output, min, mid);
    SequentialMergeSort(input, output, min, maxExclusive);
    SequentialMerge(input, output, min, mid, maxExclusive);
}

void SequentialMerge(int* input, int* output, int min, int mid, int maxExclusive){
    int lowIdx = min;
    int highIdx = mid;
    int i;
    for (i = mid; i < maxExclusive; i++){
        if (lowIdx >= mid){
            output[i] = input[highIdx];
            highIdx++;
        }else if (highIdx >= maxExclusive){
            output[i] = input[lowIdx];
            lowIdx++;
        }else if (input[lowIdx] < input[highIdx]){
            output[i] = input[lowIdx];
            lowIdx++;
        }else{
            output[i] = input[highIdx];
            highIdx++;     
        }
    }
}