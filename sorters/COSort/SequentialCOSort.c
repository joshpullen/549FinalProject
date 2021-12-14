#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "SequentialCOSort.h"
#include "../SequentialMergeSort.h"
#include "bucket.c"


// Can be modified to the version from the appendix of the paper for better span
int* prefixSum(int* input, int rows, int cols){
    // sequential
    int* output = (int*)malloc(sizeof(int)*rows*cols);
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (j > 0){
                output[i*cols+j] = output[i*cols+j-1] + input[i*cols+j-1];
            }else{
                output[i*cols] = 0;
            }
        }
    }
    return output;
}

// cache-optimal matrix transpose
void transpose(int* input, int* output, int minRow, int maxRow, int minCol, int maxCol, int inputRows, int inputCols){
    if (maxRow-minRow == 1 && maxCol-minCol == 1){
        output[minCol*inputRows+minRow] = input[minRow*inputCols+minCol];
        return;
    }
    // if width > height, split input in half by vertical line, output in half by horizontal line
    if (maxRow-minRow < maxCol-minCol){
        transpose(input, output, minRow, maxRow, minCol, (minCol+maxCol)/2, inputRows, inputCols);
        transpose(input, output, minRow, maxRow, (minCol+maxCol)/2, maxCol, inputRows, inputCols);
    }else{
        transpose(input, output, minRow, (minRow+maxRow)/2, minCol, maxCol, inputRows, inputCols);
        transpose(input, output, (minRow+maxRow)/2, maxRow, minCol, maxCol, inputRows, inputCols);
    }
}

// slightly different since it must read only the lengths from M.
// M: hxhx2 matrix. L_T: hx(h+1) matrix, where the last column should all be 0s. 
void MtoL_transpose(int* M, int* L_T, int minRow, int maxRow, int minCol, int maxCol, int numRows, int numCols){
    if (maxRow-minRow == 1 && maxCol-minCol == 1){
        if (minRow == numRows){
            L_T[minCol*(numRows+1)+minRow] = 0;
        }else{
            L_T[minCol*(numRows+1)+minRow] = M[minRow*2*numCols+2*minCol+1];
        }
        return;
    }
    // if width > height, split input in half by vertical line, output in half by horizontal line
    if (maxRow-minRow < maxCol-minCol){
        MtoL_transpose(M, L_T, minRow, maxRow, minCol, (minCol+maxCol)/2, numRows, numCols);
        MtoL_transpose(M, L_T, minRow, maxRow, (minCol+maxCol)/2, maxCol, numRows, numCols);
    }else{
        MtoL_transpose(M, L_T, minRow, (minRow+maxRow)/2, minCol, maxCol, numRows, numCols);
        MtoL_transpose(M, L_T, (minRow+maxRow)/2, maxRow, minCol, maxCol, numRows, numCols);
    }
}

// calculate M from COSORT (start location in sorted set (input) for bucket j, amount of items to go into bucket j)
// expects (sqrt n)-1 sorted pivots. partitions input into ranges based on the pivots.
// each range is {index, length}.
int* split(int* input, int* partitionSet, int inputLength, int partitionSize){
    int* output = malloc(sizeof(int)*(partitionSize+1)*2);
    output[0] = 0;
    output[1] = BinarySearch(input, partitionSet[0], inputLength);
    int totalElements = output[1];
    for (int i = 0; i < partitionSize-1; i++){
        output[(i+1)*2] = BinarySearch(input, partitionSet[i], inputLength);
        output[(i+1)*2+1] = BinarySearch(input, partitionSet[i+1], inputLength)-output[(i+1)*2];
        totalElements += output[(i+1)*2+1];
    }
    output[2*partitionSize] = BinarySearch(input, partitionSet[partitionSize-1], inputLength);
    output[2*partitionSize+1] = inputLength-totalElements;
    return output;
}

// B-Transpose algorithm for copying sorted things into buckets
//
void B_transpose(int* S, struct Bucket* B, int* T, int i_s, int i_b, int sCount, int bCount, int h){
    if (sCount == 1 && bCount == 1){
        writeRangeToBucket(S+i_s*h, B[i_b], T[i_s*3*h+i_b*3], T[i_s*3*h+i_b*3+1], T[i_s*3*h+i_b*3+2]);
        return;
    }else if (sCount == 1){
        B_transpose(S, B, T, i_s, i_b, sCount, bCount/2, h);
        B_transpose(S, B, T, i_s, i_b+(bCount/2), sCount, (bCount+1)/2, h);
    }else if (bCount == 1){
        B_transpose(S, B, T, i_s, i_b, sCount/2, bCount, h);
        B_transpose(S, B, T, i_s+(sCount/2), i_b, (sCount+1)/2, bCount, h);
    }else{
        B_transpose(S, B, T, i_s, i_b, sCount/2, bCount/2, h);
        B_transpose(S, B, T, i_s, i_b+(bCount/2), sCount/2, (bCount+1)/2, h);
        B_transpose(S, B, T, i_s+(sCount/2), i_b, (sCount+1)/2, bCount/2, h);
        B_transpose(S, B, T, i_s+(sCount/2), i_b+(bCount/2), (sCount+1)/2, (bCount+1)/2, h);
    }
}


int* getPivots(int* S, int n){
    int h = ceil(sqrt(n));
    int logn = ceil(log2(n));
    int sampleSize = n/logn;
    int sample[sampleSize];
    int spacing = logn;
    int initialoffset = 0;
    int additional = n%logn;
    for (int i = 0; i < sampleSize; i++){
        sample[i] = S[initialoffset+spacing*i+(additional > 0)];
        if (additional > 0){
            additional--;
        }
    }
    int sorted[sampleSize];
    SequentialMergeSort(sample, sorted, sampleSize);
    // want sqrt(n)-1 pivots
    int* finalSample = (int*) malloc(sizeof(int)*(h-1));
    spacing = sampleSize/h;
    initialoffset = spacing;
    additional = sampleSize % h;
    for (int i = 0; i < h-1; i++){
        finalSample[i] = sorted[initialoffset + spacing*i + (additional > 0)];
        if (additional > 0){
            additional--;
        }
    }
    return finalSample;
}

void stackWarning(){
    // do nothing
}

int* COSort(int* A, int n, int depth){
    if (depth > 10){
        stackWarning();
    }
    // large enough threshold that log_2(n) < sqrt(n)
    if (n <= 20){
        int* B = (int*) malloc(n*sizeof(int));
        SequentialMergeSort(A, B, n);
        return B;
    }
    int h = ceil(sqrt(n));
    int numS = ceil(n/(h+0.0));
    // recursive calls to get array S
    int* S = malloc(sizeof(int)*n);
    // int S[h][h];
    for (int i = 0; i < n/h; i++){
        // SequentialMergeSort(A+i*h, S+i*h, h);
        int* sorted = COSort(A+i*h, h, depth+1);
        memcpy(S+i*h, sorted, h*sizeof(int));
        free(sorted);
    }
    if (n % h != 0){
        // SequentialMergeSort(A+(n-(n%h)), S+(n-(n%h)), n%h);
        int* sorted = COSort(A+(n-(n%h)), n%h, depth+1);
        memcpy(S+(n-(n%h)), sorted, (n%h)*sizeof(int));
        free(sorted);
    }
    // for (int i = 0; i < n; i++){
    //     int val = A[i];
    //     int found = 0;
    //     for (int j = 0; j < n; j++){
    //         if (S[j] == val){
    //             found = 1;
    //         }
    //     }
    //     if (found == 0){
    //         printf("Warning: input element %d, at index %d, not found in array S at depth %d.\n", val, i, depth);
    //     }
    // }
    // fflush(stdout);
    // get pivots
    int* pivots = getPivots(S, n);
    // printf("Pivots:\n");
    // for (int i = 0; i < h-1; i++){
    //     printf("%d, ", pivots[i]);
    // }
    // printf("\n");
    // generate the array M
    int* M = (int*) malloc(sizeof(int)*numS*h*2);
    // int M[h][h][2];
    for (int i = 0; i < n/h; i++){
        int* spl = split(S+i*h, pivots, h, h-1);
        memcpy(M+i*h*2, spl, sizeof(int)*h*2);
        free(spl);
    }
    if (n%h != 0){
        int* spl = split(S+(n-(n%h)), pivots, n%h, h-1);
        memcpy(M+(numS-1)*h*2, spl, sizeof(int)*2*h);
        free(spl);
    }
    // free pivots once we are done with it
    // free(pivots);
    // transpose the correct things from M to get L_T
    int* L_T = (int*)malloc(sizeof(int)*(numS+1)*h);
    // int L_T[h][numS+1];
    MtoL_transpose(M, L_T, 0, numS+1, 0, h, numS, h);
    // Create O
    int* O = prefixSum(L_T, h, numS+1);
    free(L_T);
    int* O_T = (int*) malloc(sizeof(int)*h*(numS+1));
    // int O_T [h+1][h];
    transpose(O, O_T, 0, h, 0, numS+1, h, numS+1);
    free(O);
    int* T = (int*) malloc(sizeof(int)*numS*h*3);
    // int T[h][h][3];
    for(int i = 0; i < numS; i++){
        for (int j = 0; j < h; j++){
            // T[i_s][i_b][0]: index in S[i_s] to start in bucket B[i_b]
            T[i*3*h+j*3] = M[i*2*h+2*j];
            // T[i_s][i_b][1]: index in B[i_b] to write elements from S[i_s] to
            T[i*3*h+j*3+1] = O_T[i*(h)+j]; 
            // T[i_s][i_b][2]: amount of items to write from S[i_s] to B[i_b]
            T[i*3*h+j*3+2] = M[i*2*h+2*j+1];
        }
    }
    // M : numS rows, h cols
    // L: numS+1 rows, h cols
    // L_T: h cols, numS+1 rows
    // O: h cols, numS+1 rows
    // O_T: numS+1 rows, h cols
    // where O_T[i][j] = i'th S, j'th bucket, index into jth bucket
    free(M);
    int* bucketArray = malloc(n*sizeof(int));
    struct Bucket* B = createBuckets(O_T+numS*h, bucketArray, n);
    free(O_T);
    B_transpose(S, B, T, 0, 0, numS, h, h);
// it do be stack overflowing tho :flushed:
    free(T);
    free(S);
    // for (int i = 0; i < n; i++){
    //     int val = S[i];
    //     int found = 0;
    //     for (int j = 0; j < n; j++){
    //         if (bucketArray[j] == val){
    //             found = 1;
    //         }
    //     }
    //     if (found == 0){
    //         printf("Warning: input element %d, at index %d, not found in bucket array at depth %d.\n", val, i, depth);
    //     }
    // }
    // fflush(stdout);
    // printf("bucket array contents:\n");
    // for (int i = 0; i < n; i++){
    //     printf("%d, ", bucketArray[i]);
    // }
    // printf("\n");
    // fflush(stdout);
    int* output = malloc(n*sizeof(int));
    for (int i = 0; i < h; i++){
        if (B[i].length > 0){            
            // sometimes B[i].array is uninitialized/empty
            // despite B_transpose having written to every spot
            int* recursiveOutput = COSort(B[i].array + B[i].offset, B[i].length, depth+1);
            memcpy(output + B[i].offset, recursiveOutput, B[i].length*sizeof(int));
            free(recursiveOutput);
            // SequentialMergeSort(B[i].array+B[i].offset, output+B[i].offset, B[i].length);
        }
    }
    freeBuckets(B, h);
    free(bucketArray);
    return output;


}