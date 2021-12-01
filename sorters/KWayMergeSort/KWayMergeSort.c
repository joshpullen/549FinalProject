#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "heap.h"

// void kWayMerge(int **subarrays,int *result, int k) {
//     // Initialize the heap,  and insert the first elements from k subarrays in the heap
//     struct Heap* heap = createHeap(k);
//     int indices[k]; // indices[i] = the current index in subarrays[i]
//     int finishedCount = 0; // count(indices[i] == n)
//     int resultIdx = 0;
//     int length[k]; // Length of each subarray
//     for (int i = 0; i < k; ++ i) length[i] = sizeof(subarrays[i]) / sizeof(int);
//     memset(indices, 0, sizeof(int) * k);
//     for (int i = 0; i < k; ++ i) {
//         struct Element *element = (struct Element*)malloc(sizeof(struct Element));
//         element->value = subarrays[i][0];
//         element->arrayIndex = i;
//         insert(heap, element);
//     }
//     // Find and remove the minimum element, and insert a new element from the same subarray
//     // Execute until all elements have been merged
//     while (1) {
//         if (finishedCount == k) {
//             while (heap->currentSize != 0) result[resultIdx ++ ] = removeMin(heap)->value;
//             break;
//         }
//         struct Element *element = removeMin(heap);
//         int curIdx = element->arrayIndex;
//         result[resultIdx ++ ] = element->value;
//         // Try to insert a new element from the same subarray
//         if (indices[curIdx] < length[curIdx]) {
//             struct Element *element = (struct Element*)malloc(sizeof(struct Element));
//             element->value = subarrays[curIdx][indices[curIdx] ++ ];
//             element->arrayIndex = curIdx;
//             insert(heap, element);
//         }
//         // There is no element in the same subarray, so we need to find another one
//         else {
//             ++ finishedCount;
//             while (1) {
//                 curIdx = (curIdx + 1) % k;
//                 if (indices[curIdx] >= length[curIdx]) continue;
//                 else {
//                     struct Element *element = (struct Element*)malloc(sizeof(struct Element));
//                     element->value = subarrays[curIdx][indices[curIdx] ++ ];
//                     element->arrayIndex = curIdx;
//                     insert(heap, element);
//                     break;
//                 }
//             }

//         }
//     }
// }

// partition: indicates max indices (exclusive) of k subarrays of sorted
// ranges of subarrays of sorted: [0, partition[0]-1], [partition[0], partition[1]-1], ... [partition[k-2], partition[k-1]-1]  (partition[k-1] = length)
void kWayMerge(int *sorted, int *result, int k, int *partition) {
    // Initialize the heap,  and insert the first elements from k subarrays in the heap
    struct Heap* heap = createHeap(k);
    int indices[k]; // indices[i] = the current index of ith subarray
    int finishedCount = 0; // count(indices[i] == partition[i])
    int resultIdx = 0;
    // Init indices (starting points of subarrays)
    for (int i = 0; i < k; ++ i) {
        if (i == 0) indices[i] = 0;
        else indices[i] = partition[i - 1];
    }
    for (int i = 0; i < k; ++ i) {
        struct Element *element = (struct Element*)malloc(sizeof(struct Element));
        // element->value = subarrays[i][0];
        element->value = sorted[indices[i] ++ ];
        element->arrayIndex = i;
        insert(heap, element);
        if (indices[i] == partition[i]) ++ finishedCount;
    }
    // Find and remove the minimum element, and insert a new element from the same subarray
    // Execute until all elements have been merged
    while (1) {
        if (finishedCount == k) {
            while (heap->currentSize != 0) result[resultIdx ++ ] = removeMin(heap)->value;
            break;
        }
        struct Element *element = removeMin(heap);
        int curArray = element->arrayIndex;
        result[resultIdx ++ ] = element->value;
        // Try to insert a new element from the same subarray
        if (indices[curArray] < partition[curArray]) {
            struct Element *element = (struct Element*)malloc(sizeof(struct Element));
            // element->value = subarrays[curArray][indices[curArray] ++ ];
            element->value = sorted[indices[curArray] ++ ];
            element->arrayIndex = curArray;
            insert(heap, element);
            if (indices[curArray] == partition[curArray]) ++ finishedCount;
        }
            // There is no element in the same subarray, so we need to find another one
        else {
//            if (++ finishedCount == k) continue;
            while (1) {
                curArray = (curArray + 1) % k;
                if (indices[curArray] == partition[curArray]) continue;
                else {
                    struct Element *element = (struct Element*)malloc(sizeof(struct Element));
                    // element->value = subarrays[curArray][indices[curArray] ++ ];
                    element->value = sorted[indices[curArray] ++ ];
                    element->arrayIndex = curArray;
                    insert(heap, element);
                    if (indices[curArray] == partition[curArray]) ++ finishedCount;
                    break;
                }
            }

        }
    }
    freeHeap(heap);
}

void kWayMergeSort(int *input, int *result, int k, int length) {
    int i;
//    for (i = 0; i < length; ++ i) printf("%d ", *(input + i));
//    printf("\n");
    if (length == 1) {
        result[0] = input[0];
        return;
    }
    if (length < k){
        kWayMergeSort(input, result, length, length);
        return;
    }
    int* extra = (int*)malloc(length*sizeof(int));
    int lengthOfSubarray = length / k;
    int partition[k];
    for (i = 0; i < k; ++ i) {
        if (i == 0) partition[i] = lengthOfSubarray;
        else {
            if (i == k - 1 && partition[i - 1] + lengthOfSubarray < length) partition[i] = length;
            else partition[i] = partition[i - 1] + lengthOfSubarray;
        }
    }
//    printf("partition: ");
//    for (i = 0; i < k; ++ i) printf("%d ", partition[i]);
//    printf("\n");
    for (i = 0; i < k; ++ i) {
        if (i == 0) kWayMergeSort(input, extra, k, partition[i]);
        else kWayMergeSort(input + partition[i - 1], extra + partition[i - 1], k, partition[i] - partition[i - 1]);
    }
//    for (i = 0; i < length; ++ i) printf("%d ", *(input + i));
//    printf("\n");
//    for (i = 0; i < length; ++ i) printf("%d ", *(extra + i));
//    printf("\n");
    kWayMerge(extra, result, k, partition);
    free(extra);
}

//int main(int argc, char** argv) {
//    int arr[] = {6,5,4,3,2,1};
//    int length = sizeof(arr) / sizeof(int);
//    int output[length], i = 0;
//    int k = 3;
//    kWayMergeSort(arr, output, k, length);
//    for (i = 0; i < length; ++ i) printf("%d ", output[i]);
//    printf("\n");
//}
