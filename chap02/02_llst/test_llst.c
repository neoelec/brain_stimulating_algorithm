#include <stdio.h>
#include <stdlib.h>

#include <rcn/llst.h>

struct llst_entry {
    struct sgll_node node;
    const char *data;
};

static void llst_push_entry(struct llst *stack, const char *data)
{
    struct llst_entry *entry;

    entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate memory for stack entry\n");
        exit(EXIT_FAILURE);
    }

    entry->data = data;
    llst_push(stack, &entry->node);
}

static const char *llst_pop_entry(struct llst *stack)
{
    struct sgll_node *node;
    struct llst_entry *entry;
    const char *data;
    int err;

    err = llst_pop(stack, &node);
    if (err != 0) {
        fprintf(stderr, "Failed to pop entry from stack (%d)\n", err);
        return "NiL";
    }

    entry = container_of(node, struct llst_entry, node);
    data = entry->data;
    free(entry);

    return data;
}

static const char *llst_top_entry(struct llst *stack)
{
    struct sgll_node *node;
    struct llst_entry *entry;
    int err;

    err = llst_top(stack, &node);
    if (err != 0) {
        fprintf(stderr, "Failed to get the top entry from stack (%d)\n", err);
        return "NiL";
    }

    entry = container_of(node, struct llst_entry, node);

    return entry->data;
}

static struct llst *llst_create_stack(void)
{
    struct llst *stack;

    stack = malloc(sizeof(*stack));
    if (stack == NULL) {
        fprintf(stderr, "Failed to allocate memory for stack\n");
        exit(EXIT_FAILURE);
    }

    llst_initialize(stack);

    return stack;
}

static void llst_destroy_stack(struct llst *stack)
{
    while (!llst_is_empty(stack))
        llst_pop_entry(stack);

    free(stack);
}

int main(void)
{
    size_t count;
    struct llst *stack;

    stack = llst_create_stack();

    llst_push_entry(stack, "abc");
    llst_push_entry(stack, "def");
    llst_push_entry(stack, "efg");
    llst_push_entry(stack, "hij");

    count = llst_count(stack);
    printf("Count: %zu, Top: %s\n\n", count, llst_top_entry(stack));

    while (true) {
        const char *data = llst_pop_entry(stack);

        printf("Popped: %s, ", data);

        if (!llst_is_empty(stack)) {
            printf("Current Top: %s\n", llst_top_entry(stack));
        } else {
            printf("Stack Is Empty.\n");
            break;
        }
    }

    llst_destroy_stack(stack);

    return 0;
}
