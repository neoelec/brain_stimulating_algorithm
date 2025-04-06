#include <stdio.h>
#include <stdlib.h>

#include <rcn/llqu.h>

struct dbll_entry {
    struct dbll_node node;
    const char *data;
};

static void llqu_enqueue_entry(struct dbll *queue, const char *data)
{
    struct dbll_entry *entry;

    entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate memory for queue entry\n");
        exit(EXIT_FAILURE);
    }

    entry->data = data;
    llqu_enqueue(queue, &entry->node);
}

static const char *llqu_dequeue_entry(struct dbll *queue)
{
    struct dbll_node *node;
    struct dbll_entry *entry;
    const char *data;
    int err;

    err = llqu_dequeue(queue, &node);
    if (err != 0) {
        fprintf(stderr, "Failed to dequeue entry from queue (%d)\n", err);
        return "NiL";
    }

    entry = container_of(node, struct dbll_entry, node);
    data = entry->data;
    free(entry);

    return data;
}

static struct dbll *llqu_create_queue(void)
{
    struct dbll *queue;

    queue = malloc(sizeof(*queue));
    if (queue == NULL) {
        fprintf(stderr, "Failed to allocate memory for queue\n");
        exit(EXIT_FAILURE);
    }

    llqu_initialize(queue);

    return queue;
}

static void llqu_destroy_queue(struct dbll *queue)
{
    while (!llqu_is_empty(queue))
        llqu_dequeue_entry(queue);

    free(queue);
}

int main(void)
{
    struct dbll *queue;

    queue = llqu_create_queue();

    llqu_enqueue_entry(queue, "abc");
    llqu_enqueue_entry(queue, "def");
    llqu_enqueue_entry(queue, "efg");
    llqu_enqueue_entry(queue, "hij");

    printf("Count: %zu\n\n", queue->count);

    while (!llqu_is_empty(queue))
        printf("Dequeue: %s\n", llqu_dequeue_entry(queue));

    llqu_destroy_queue(queue);

    return 0;
}
