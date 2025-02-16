#include <stdio.h>
#include <stdlib.h>

#include <rcn/heap.h>

static int heap_compare(const void *_a, const void *_b)
{
    intptr_t a = (intptr_t)_a;
    intptr_t b = (intptr_t)_b;

    if (a > b)
        return 1;
    else if (a < b)
        return -1;
    else
        return 0;
}

static void heap_insert_entry(struct heap *heap, intptr_t data)
{
    heap_insert_i(heap, data);
}

static intptr_t heap_remove_entry(struct heap *heap)
{
    intptr_t data;
    int err;

    err = heap_remove_i(heap, &data);
    if (err != 0) {
        fprintf(stderr, "Failed to remove entry from heap (%d)\n", err);
        return err;
    }

    return data;
}

static void heap_print_entry(void *entry, void *private)
{
    intptr_t data = (intptr_t)entry;

    printf("%ld ", data);
}

static void heap_print_heap(struct heap *heap)
{
    heap_forward(heap, heap_print_entry, NULL);
    printf("\n");
}

static struct heap *heap_create_heap(void)
{
    struct heap *heap;

    heap = malloc(sizeof(*heap));
    if (heap == NULL) {
        fprintf(stderr, "Failed to allocate memory for heap\n");
        exit(EXIT_FAILURE);
    }

    heap_initialize(heap, heap_compare);

    return heap;
}

static void heap_destroy_heap(struct heap *heap)
{
    while (!heap_is_empty(heap))
        heap_remove_entry(heap);

    free(heap);
}

int main(void)
{
    struct heap *heap;

    heap = heap_create_heap();

    heap_insert_entry(heap, 12);
    heap_insert_entry(heap, 87);
    heap_insert_entry(heap, 111);
    heap_insert_entry(heap, 34);
    heap_insert_entry(heap, 15);
    heap_insert_entry(heap, 75);
    heap_print_heap(heap);

    heap_remove_entry(heap);
    heap_print_heap(heap);

    heap_remove_entry(heap);
    heap_print_heap(heap);

    heap_remove_entry(heap);
    heap_print_heap(heap);

    heap_remove_entry(heap);
    heap_print_heap(heap);

    heap_remove_entry(heap);
    heap_print_heap(heap);

    heap_destroy_heap(heap);

    return 0;
}
