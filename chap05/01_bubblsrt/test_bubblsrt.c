#include <stdio.h>

#include <rcn/bubblsrt.h>

static int int_compare(const void *a, const void *b)
{
    return *(const int *)a - *(const int *)b;
}

int main(void)
{
    int data[] = { 6, 4, 2, 3, 1, 5 };
    size_t i;

    bubblsrt(data, NR_ELEM(data), sizeof(data[0]), int_compare);

    for (i = 0; i < NR_ELEM(data); i++)
        printf("%d ", data[i]);

    printf("\n");

    return 0;
}
