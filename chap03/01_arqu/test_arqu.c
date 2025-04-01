#include <stdio.h>
#include <stdlib.h>

#include <rcn/arqu.h>

static void arqu_enqueue_entry(struct arqu *queue, intptr_t data)
{
    arqu_enqueue_i(queue, data);
}

static intptr_t arqu_dequeue_entry(struct arqu *queue)
{
    intptr_t data;
    int err;

    err = arqu_dequeue_i(queue, &data);
    if (err != 0) {
        fprintf(stderr, "Failed to dequeue entry from queue (%d)\n", err);
        return err;
    }

    return data;
}

static struct arqu *arqu_create_queue(size_t nr_entries)
{
    struct arqu *queue;

    queue = malloc(ARQU_BYTES(nr_entries));
    if (queue == NULL) {
        fprintf(stderr, "Failed to allocate memory for queue\n");
        exit(EXIT_FAILURE);
    }

    arqu_initialize(queue, nr_entries);

    return queue;
}

static void arqu_destroy_queue(struct arqu *queue)
{
    free(queue);
}

int main(void)
{
    int i;
    struct arqu *queue;

    queue = arqu_create_queue(10);

    arqu_enqueue_entry(queue, 1);
    arqu_enqueue_entry(queue, 2);
    arqu_enqueue_entry(queue, 3);
    arqu_enqueue_entry(queue, 4);

    for (i = 0; i < 3; i++) {
        printf("Dequeue: %ld, ", arqu_dequeue_entry(queue));
        printf("Front: %zu, Rear: %zu\n", queue->front, queue->rear);
    }

    i = 100;
    while (!arqu_is_full(queue))
        arqu_enqueue_entry(queue, i++);

    printf("Capacity: %zu, Count:  %zu\n\n", queue->nr_entries,
           arqu_count(queue));

    while (!arqu_is_empty(queue)) {
        printf("Dequeue: %ld, ", arqu_dequeue_entry(queue));
        printf("Front: %zu, Rear: %zu\n", queue->front, queue->rear);
    }

    arqu_destroy_queue(queue);

    return 0;
}
