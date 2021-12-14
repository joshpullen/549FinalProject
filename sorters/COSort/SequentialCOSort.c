#include <stdio.h>
#include <stdlib.h>
#include "COSort.h"


// Can be modified to the version from the appendix of the paper for better span
int** prefixSum(int** input, int width, int height){
    // sequential
    int** output = new *int[height];
    for (int i = 0; i < height; i++){
        output[i] = new int[width];
        for (int j = 0; j < width; j++){
            if (j > 0){
                output[i][j] = output[i][j-1] + input[i][j];
            }
        }
    }
    return output;
}

// cache-optimal matrix transpose
void transpose(int** input, int** output, int minRow, int maxRow, int minCol, int maxCol){
    if (maxRow-minRow == 1 && maxCol-minCol == 1){
        output[minCol][minRow] = input[minRow][minCol];
        return;
    }
    // if width > height, split input in half by vertical line, output in half by horizontal line
    if (maxRow-minRow < maxCol-minCol){
        transpose(input, output, minRow, maxRow, minCol, (minCol+maxCol)/2);
        transpose(input, output, minRow, maxRow, (minCol+maxCol)/2, maxCol);
    }else{
        transpose(input, output, minRow, (minRow+maxRow)/2, minCol, maxCol);
        transpose(input, output, (minRow+maxRow)/2, maxRow, minCol, maxCol);
    }
}

// calculate M from COSORT (start location in sorted set (input) for bucket j, amount of items to go into bucket j)
// to make this better, will want to return two separate arrays rather than one combined array
**int split(int* input, int* partitionSet, int inputLength, int partitionSize){
    int** output = new *int[partitionSize];
    int totalElements = 0;
    for (int i = 0; i < partitionSize-1; i++){
        output[i] = new int[2];
        output[i][0] = binarySearch(input, partitionSet[i]);
        output[i][1] = binarySearch(input, partitionSet[i+1])-output[i][0];
        totalElements += output[i][1];
    }
    output[partitionSize-1] = new int[2];
    output[partitionSize-1][0] = binarySearch(input, partitionSet[partitionSize-1]);
    output[partitionSize-1][1] = inputLength-totalElements;
}

// B-Transpose algorithm for copying sorted things into buckets
void B_transpose(int** S, int** B, int*** T, int i_s, int i_b, int n){
    if (n == 1){
        memcpy(B[T[i_s][i_b][1]], S[T[i_s][i_b][0]], T[i_s][i_b][3]*sizeof(int));
        return;
    }
    B_transpose(S, B, T, i_s, i_b, n/2);
    B_tranpose(S, B, T, i_s, i_b+(n/2), n/2);
    B_transpose(S, B, T, i_s+(n/2), i_b, n/2);
    B_transpose(S, B, T, i_s+(n/2), i_b+(n/2), n/2);
}


int* COSort(int* A, int n){

}