#include <stdlib.h>
#include <stdio.h>

// gcc -c main.cpp
// gcc -o main main.o

void printarray(int *a, int len)
{
    for (int i = 0; i < len; ++i)
    {
        printf("%d\t", a[i]);
    }
    printf("\n");
}

void qsort(int *array, int start, int end)
{

    if (end <= start)
        return;
    int len = end - start + 1;
    if (len <= 1)
        return;

    int mid = len / 2 + start;
    int j = start;
    int k = end;

    while(j <= k)
    {
        if (j < mid)
        {
            if (array[mid] > array[j])
            {
                int temp = array[j];
                array[j] = array[mid];
                array[mid] = temp;
                mid = j;
            }
            ++j;
        }
        if (k > mid)
        {
            if (array[mid] < array[k])
            {
                int temp = array[k];
                array[k] = array[mid];
                array[mid] = temp;
                mid = k;
            }
            --k;
        }

    }

    qsort(array, mid + 1, end);
    qsort(array, start, mid - 1);
}

int main(int argc, char *argv[])
{
    int array[10] = {7, 3, 8, 6, 9, 2, 1, 0, 4, 5};
    printarray(array, 10);
    qsort(array, 0, 9);
    printarray(array, 10);
    return 1;
}




