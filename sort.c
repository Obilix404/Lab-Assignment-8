//Jonathan Vega Lab Assignment 8
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int extraMemoryAllocated;

void swap(int* a, int* b) {

    int temp = *a;
    *a       = *b;
    *b       = temp;
}

void heapify(int arr[], int n, int i) {

    //find the biggest among the root, left child, right child

    int largest = i;

    int left  = 2 * i + 1;
    int right = 2 * i + 2;

    //cases to check if each child is bigger

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    //case if largest is not root

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n) {

    //build the max heap

    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    //implement sorting

    for (int i = n - 1; i >= 0; i--) {
        swap(&arr[0], &arr[i]);

        heapify(arr, i, 0);
    }
}

void merge(int pData[], int left, int middle, int right) {
    int  i, j, k;
    int  number1     = middle - left + 1;
    int  number2     = right - middle;
    int* left_array  = malloc(sizeof(int) * number1);
    int* right_array = malloc(sizeof(int) * number2);

    extraMemoryAllocated += (sizeof(int) * number1) + (sizeof(int) * number2);

    for (i = 0; i < number1; i++)
        left_array[i] = pData[left + i];
    for (j = 0; j < number2; j++)
        right_array[j] = pData[middle + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < number1 && j < number2) {
        if (left_array[i] <= right_array[j]) {
            pData[k] = left_array[i];
            i++;
        } else {
            pData[k] = right_array[j];
            j++;
        }
        k++;
    }

    while (i < number1) {
        pData[k] = left_array[i];
        i++;
        k++;
    }

    while (j < number2) {
        pData[k] = right_array[j];
        j++;
        k++;
    }
    free(left_array);
    free(right_array);
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        mergeSort(pData, left, middle);
        mergeSort(pData, middle + 1, right);

        merge(pData, left, middle, right);
    }
}

// parses input file to an integer array
int parseData(char* inputFileName, int** ppData) {
    FILE* inFile = fopen(inputFileName, "r");
    int   dataSz = 0;
    int   i, n, *data;
    *ppData = NULL;

    if (inFile) {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (int*)malloc(sizeof(int) * dataSz);
        // Implement parse data block
        if (*ppData == NULL) {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i) {
            fscanf(inFile, "%d ", &n);
            data  = *ppData + i;
            *data = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz) {
    int i, sz = dataSz - 100;
    printf("\tData:\n\t");
    for (i = 0; i < 100; ++i) {
        printf("%d ", pData[i]);
    }
    printf("\n\t");

    for (i = sz; i < dataSz; ++i) {
        printf("%d ", pData[i]);
    }
    printf("\n\n");
}

int main(void) {
    clock_t start, end;
    int     i;
    double  cpu_time_used;
    char*   fileNames[] = {"input1.txt", "input2.txt", "input3.txt", "input4.txt"};

    for (i = 0; i < 4; ++i) {
        int *pDataSrc, *pDataCopy;
        int  dataSz = parseData(fileNames[i], &pDataSrc);

        if (dataSz <= 0)
            continue;

        pDataCopy = (int*)malloc(sizeof(int) * dataSz);

        printf("---------------------------\n");
        printf("Dataset Size : %d\n", dataSz);
        printf("---------------------------\n");

        printf("Heap Sort:\n");
        memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
        extraMemoryAllocated = 0;
        start                = clock();
        heapSort(pDataCopy, dataSz);
        end           = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
        printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
        printArray(pDataCopy, dataSz);

        printf("Merge Sort:\n");
        memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
        extraMemoryAllocated = 0;
        start                = clock();
        mergeSort(pDataCopy, 0, dataSz - 1);
        end           = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
        printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
        printArray(pDataCopy, dataSz);

        free(pDataCopy);
        free(pDataSrc);
    }
}