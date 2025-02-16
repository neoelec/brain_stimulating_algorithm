#include <stdio.h>
#include <stdlib.h>

#ifndef NR_ELEM
#define NR_ELEM(a) ((sizeof(a)) / (sizeof((a)[0])))
#endif

static int int_compare(const void *a, const void *b)
{
    return *(const int *)a - *(const int *)b;
}

int main(void)
{
    int data[] = { 6, 4, 2, 3, 1, 5 };
    size_t i;

    qsort(data, NR_ELEM(data), sizeof(data[0]), int_compare);

    for (i = 0; i < NR_ELEM(data); i++)
        printf("%d ", data[i]);

    printf("\n");

    return 0;
}
