#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <rcn/bubblsrt.h>
#include <rcn/heapsort.h>
#include <rcn/instnsrt.h>
#include <rcn/mergesrt.h>
#include <rcn/quicksrt.h>
#include <rcn/radixsrt.h>
#include <rcn/shellsrt.h>
#include <rcn/slctnsrt.h>

#define MAX_ELEM 10000

static int random_data[MAX_ELEM];
static int ref_data[MAX_ELEM];
static int data[MAX_ELEM];

static int int_compare(const void *_a, const void *_b)
{
    int a = *(const int *)_a;
    int b = *(const int *)_b;

    return a - b;
}

static void __prepare_random_data(void)
{
    size_t i;

    for (i = 0; i < NR_ELEM(random_data); i++)
        random_data[i] = rand() % NR_ELEM(random_data);

    memcpy(ref_data, random_data, sizeof(random_data));
    qsort(ref_data, NR_ELEM(ref_data), sizeof(ref_data[0]), int_compare);
}

static void ____benchmark_sort_function(
    const char *name,
    void (*sort)(void *, size_t, size_t, int (*)(const void *, const void *)))
{
    clock_t start, end, time, seconds;
    bool result;

    memcpy(data, random_data, sizeof(data));

    start = clock();
    sort(data, NR_ELEM(data), sizeof(data[0]), int_compare);
    end = clock();

    time = end - start;
    seconds = time / CLOCKS_PER_SEC;

    result = !memcmp(data, ref_data, sizeof(data));

    printf("[%s] %s sort took %ld processor time. (%ld seconds)\n",
           result ? "PASS" : "FAIL", name, time, seconds);
}

static void ____benchmark_sort_functionalt(const char *name,
                                           unsigned long long order,
                                           void (*sort)(void *, size_t, size_t,
                                                        unsigned long long))
{
    clock_t start, end, time, seconds;
    bool result;

    memcpy(data, random_data, sizeof(data));

    start = clock();
    sort(data, NR_ELEM(data), sizeof(data[0]), order);
    end = clock();

    time = end - start;
    seconds = time / CLOCKS_PER_SEC;

    result = !memcmp(data, ref_data, sizeof(data));

    printf("[%s] %s (%llu) sort took %ld processor time. (%ld seconds)\n",
           result ? "PASS" : "FAIL", name, order, time, seconds);
}

#define __benchmark_sort_function(sort) ____benchmark_sort_function(#sort, sort)
#define __benchmark_sort_functionalt(sort, order) \
    ____benchmark_sort_functionalt(#sort, order, sort)

int main(void)
{
    srand(time(NULL));

    __prepare_random_data();

    __benchmark_sort_function(bubblsrt);
    __benchmark_sort_function(heapsort);
    __benchmark_sort_function(instnsrt);
    __benchmark_sort_function(mergesrt);
    __benchmark_sort_function(slctnsrt);
    __benchmark_sort_function(shellsrt);
    __benchmark_sort_function(quicksrt);
    __benchmark_sort_function(quicksrt_m);
    __benchmark_sort_function(quicksrt_r);
    __benchmark_sort_function(quicksrt_s);
    __benchmark_sort_functionalt(radixsrt, 1);
    __benchmark_sort_functionalt(radixsrt, 2);
    __benchmark_sort_functionalt(radixsrt, 3);
    __benchmark_sort_functionalt(radixsrt, 4);
    __benchmark_sort_functionalt(radixsrt, 5);

    __benchmark_sort_function(qsort);

    return 0;
}
