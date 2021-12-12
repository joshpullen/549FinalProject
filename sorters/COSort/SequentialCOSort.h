int* COSort(int* input, int n);

void B_transpose(int** S, int** B, int*** T, int i_s, int i_b, int n);

void transpose(int** input, int** output, int minRow, int maxRow, int minCol, int maxCol);

int** split(int* input, int* partitionSet);

int** prefixSum(int** input, int width, int height);