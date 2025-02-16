#include <stdio.h>
#include <stdlib.h>

#include <rcn/arst.h>

static void arst_push_entry(struct arst *stack, intptr_t data)
{
    arst_push_i(stack, data);
}

static intptr_t arst_pop_entry(struct arst *stack)
{
    intptr_t data;
    int err;

    err = arst_pop_i(stack, &data);
    if (err != 0) {
        fprintf(stderr, "Failed to pop entry from stack (%d)\n", err);
        return err;
    }

    return data;
}

static intptr_t arst_top_entry(struct arst *stack)
{
    intptr_t data;
    int err;

    err = arst_top_i(stack, &data);
    if (err != 0) {
        fprintf(stderr, "Failed to get the top entry from stack (%d)\n", err);
        return err;
    }

    return data;
}

static struct arst *arst_create_stack(size_t nr_entries)
{
    struct arst *stack;

    stack = malloc(ARST_BYTES(nr_entries));
    if (stack == NULL) {
        fprintf(stderr, "Failed to allocate memory for stack\n");
        exit(EXIT_FAILURE);
    }

    arst_initialize(stack, nr_entries);

    return stack;
}

static void arst_destroy_stack(struct arst *stack)
{
    free(stack);
}

int main(void)
{
    size_t i;
    struct arst *stack;

    stack = arst_create_stack(10);

    arst_push_entry(stack, 3);
    arst_push_entry(stack, 37);
    arst_push_entry(stack, 11);
    arst_push_entry(stack, 12);

    printf("Capacity: %zu, Count: %zu, Top: %ld\n\n", stack->nr_entries,
           arst_count(stack), arst_top_entry(stack));

    for (i = 0; i < 4; i++) {
        if (arst_is_empty(stack))
            break;

        printf("Popped: %ld, ", arst_pop_entry(stack));

        if (!arst_is_empty(stack))
            printf("Current Top: %ld\n", arst_top_entry(stack));
        else
            printf("Stack Is Empty.\n");
    }

    arst_destroy_stack(stack);

    return 0;
}
