#include <stdio.h>
#include <stdlib.h>

#include <rcn/lldq.h>

struct lldq_entry {
    struct dbll_node node;
    int data;
};

static struct lldq_entry *lldq_create_entry(int data)
{
    struct lldq_entry *entry;

    entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate memory for deque entry\n");
        exit(EXIT_FAILURE);
    }

    entry->data = data;

    return entry;
}

static void lldq_print_entry(struct dbll_node *node, void *private)
{
    ssize_t *idx = private;
    struct lldq_entry *entry = container_of(node, struct lldq_entry, node);

    printf("deque[%zd] : %d\n", (*idx)++, entry->data);
}

int main(void)
{
    ssize_t i;
    struct dbll __deque, *deque = &__deque;
    struct dbll_node *node;
    struct lldq_entry *entry;
    ssize_t count;

    lldq_initialize(deque);

    /* 노드 5개 추가 */
    for (i = 0; i < 5; i++) {
        entry = lldq_create_entry(i);
        lldq_push_rear(deque, &entry->node);
    }

    /* 뎈 출력 */
    i = 0;
    lldq_forward(deque, lldq_print_entry, &i);

    /* 뎈의 세 번째 노드 뒤에 새 노드 삽입 */
    printf("\nInserting 3000 After [2]...\n\n");
    entry = lldq_create_entry(3000);
    lldq_insert(deque, 3, &entry->node);

    /* 뎈 출력 */
    i = 0;
    lldq_forward(deque, lldq_print_entry, &i);

    printf("\nRemoving Node at 2...\n");
    lldq_remove(deque, 2, &node);
    free(container_of(node, struct lldq_entry, node));

    /* 뎈 출력 */
    /* (노드 수의 2배만큼 루프를 돌며 환형임을 확인한다.) */
    count = lldq_count(deque);
    for (i = 0; i < count * 2; i++) {
        lldq_get(deque, i, &node);
        entry = container_of(node, struct lldq_entry, node);
        printf("deque[%zd] : %d\n", i, entry->data);
    }

    /* 뎈 뒤집기 */
    printf("\nReversing Deque...\n");
    lldq_reverse(deque);

    /* 뎈 출력 */
    i = 0;
    lldq_forward(deque, lldq_print_entry, &i);

    /* 뎈 회전 */
    printf("\nRotating Deque...\n");
    lldq_rotate(deque, -1);

    /* 뎈 출력 */
    i = 0;
    lldq_forward(deque, lldq_print_entry, &i);

    /* 뎈 출력 */
    printf("\nNegative Index...\n");
    count = lldq_count(deque);
    for (i = 1; i <= count; i++) {
        lldq_get(deque, -i, &node);
        entry = container_of(node, struct lldq_entry, node);
        printf("deque[%zd] : %d\n", -i, entry->data);
    }

    /* 모든 노드를 메모리에서 제거 */
    printf("\nDestroyihng Deque...\n");
    while (!lldq_is_empty(deque)) {
        lldq_pop_rear(deque, &node);
        free(container_of(node, struct lldq_entry, node));
    }

    return 0;
}
