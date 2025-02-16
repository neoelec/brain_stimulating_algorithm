#include <stdio.h>
#include <stdlib.h>

#include <rcn/crqu.h>

static void crqu_enqueue_entry(struct crqu *queue, intptr_t data)
{
    crqu_enqueue_i(queue, data);
}

static intptr_t crqu_dequeue_entry(struct crqu *queue)
{
    intptr_t data;
    int err;

    err = crqu_dequeue_i(queue, &data);
    if (err != 0) {
        fprintf(stderr, "Failed to dequeue entry from queue (%d)\n", err);
        return err;
    }

    return data;
}

static struct crqu *crqu_create_queue(size_t nr_entries)
{
    struct crqu *queue;

    queue = malloc(CRQU_BYTES(nr_entries));
    if (queue == NULL) {
        fprintf(stderr, "Failed to allocate memory for queue\n");
        exit(EXIT_FAILURE);
    }

    crqu_initialize(queue, nr_entries);

    return queue;
}

static void crqu_destroy_queue(struct crqu *queue)
{
    free(queue);
}

int main(void)
{
    int i;
    struct crqu *queue;

    queue = crqu_create_queue(10);

    crqu_enqueue_entry(queue, 1);
    crqu_enqueue_entry(queue, 2);
    crqu_enqueue_entry(queue, 3);
    crqu_enqueue_entry(queue, 4);

    for (i = 0; i < 3; i++) {
        printf("Dequeue: %ld, ", crqu_dequeue_entry(queue));
        printf("Front: %zu, Rear: %zu\n", queue->front, queue->rear);
    }

    i = 100;
    while (!crqu_is_full(queue))
        crqu_enqueue_entry(queue, i++);

    printf("Capacity: %zu, Count:  %zu\n\n", queue->nr_entries,
           crqu_count(queue));

    while (!crqu_is_empty(queue)) {
        printf("Dequeue: %ld, ", crqu_dequeue_entry(queue));
        printf("Front: %zu, Rear: %zu\n", queue->front, queue->rear);
    }

    crqu_destroy_queue(queue);

    return 0;
}
