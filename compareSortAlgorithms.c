#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

//added for simplification of heapSort recursive functions
void heapify(int arr[], int n, int i)
{
    int temp;
    int max = i;
    int left = (i * 2) + 1;
    int right = (i * 2) + 2;

    if((left < n) && (arr[max] < arr[left]))
        max = left;

    if((right < n) && (arr[max] < arr[right]))
        max = right;

    if (max != i)
    {
    temp = arr[i];
    arr[i] = arr[max];
    arr[max] = temp;
    heapify(arr, n, max);
    }
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
    int temp, i;

    for(i = (n / 2) - 1; i >= 0; i--)
    {
        heapify(arr, n, i);
    }

    for (i = n - 1; i > 0; i--) {
        temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        heapify(arr, i, 0);
    }

}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
    if(l < r)
    {
//Section of the function dedicated to sort
    int m = (l + r) / 2;

    mergeSort(pData, l, m);
    mergeSort(pData, m + 1, r);

//Section of the function dedicated to merge
    int n1 = m - l + 1;
    int n2 = r - m;
    int x, y, z;
    int *a = (int*)Alloc(sizeof(int) * n1);
    int *b = (int*)Alloc(sizeof(int) * n2);

    for(int i = 0; i < n1; i++)
        a[i] = pData[l + i];
    for(int j = 0; j < n2; j++)
        b[j] = pData[m + 1 + j];

    x = 0;
    y = 0;
    z = l;

    while(x < n1 && y < n2)
    {
        if(a[x] <= b[y])
        {
            pData[z] = a[x];
            x++;
        } else {
            pData[z] = b[y];
            y++;
        }
        z++;
    }

    while (x < n1)
    {
        pData[z] = a[x];
        x++;
        z++;
    }

    while (y < n2)
    {
        pData[z] = b[y];
        y++;
        z++;
    }

DeAlloc(a);
DeAlloc(b);

    }
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
int item, j;

for (int i = 1; i < n; i++)
    {
    item = pData[i];

    for(j = i - 1; j >= 0; j--)
    {
        if(pData[j]>item)
            pData[j+1] = pData[j];
        else
            break;
    }
    pData[j+1] = item;
    }
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
    int *array = (int*)Alloc(sizeof(int) * n);
    int temp;

    for(int a = 0; a < n; a++)
    {
        array[a] = pData[a];
    }


for (int i = 0; i < n-1; i++)
    {
    for (int j = 0; j < n-1-i; j++)
        {
            if (pData[j] > pData[j+1])
            {
                temp = pData[j];
                pData[j] = pData[j+1];
                pData[j+1] = temp;
            }
        }
    }

    DeAlloc(array);
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
    int min_idx, temp;

    for (int i = 0; i < n-1; i++)
        {
        min_idx = i;

        for (int j = i+1; j < n; j++)
        {
            if (pData[j] < pData[min_idx])
                min_idx = j;
        }

        temp = pData[i];
        pData[i] = pData[min_idx];
        pData[min_idx] = temp;
    }
}
// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);

		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}

		int n, *data;

		for (int i = 0; i < dataSz; i++)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");

	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};

	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);

		if (dataSz <= 0)
			continue;

		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);

		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");

		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}

}
