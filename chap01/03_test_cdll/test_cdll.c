#include <stdio.h>
#include <stdlib.h>

#include <rcn/cdll.h>

struct cdll_entry {
    struct dbll_node node;
    int data;
};

static struct cdll_entry *cdll_create_entry(int data)
{
    struct cdll_entry *entry;

    entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate memory for list entry\n");
        exit(EXIT_FAILURE);
    }

    entry->data = data;

    return entry;
}

static void cdll_print_entry(struct dbll_node *node, void *private)
{
    ssize_t *idx = private;
    struct cdll_entry *entry = container_of(node, struct cdll_entry, node);

    printf("list[%zd] : %d\n", (*idx)++, entry->data);
}

int main(void)
{
    ssize_t i;
    struct cdll __list, *list = &__list;
    struct dbll_node *node;
    struct cdll_entry *entry;
    ssize_t count;

    cdll_initialize(list);

    /* 노드 5개 추가 */
    for (i = 0; i < 5; i++) {
        entry = cdll_create_entry(i);
        cdll_insert_tail(list, &entry->node);
    }

    /* 리스트 출력 */
    i = 0;
    cdll_forward(list, cdll_print_entry, &i);

    /* 리스트의 세 번째 노드 뒤에 새 노드 삽입 */
    printf("\nInserting 3000 After [2]...\n\n");
    entry = cdll_create_entry(3000);
    cdll_insert(list, 3, &entry->node);

    /* 리스트 출력 */
    i = 0;
    cdll_forward(list, cdll_print_entry, &i);

    printf("\nRemoving Node at 2...\n");
    cdll_remove(list, 2, &node);
    free(container_of(node, struct cdll_entry, node));

    /* 리스트 출력 */
    /* (노드 수의 2배만큼 루프를 돌며 환형임을 확인한다.) */
    count = cdll_count(list);
    for (i = 0; i < count * 2; i++) {
        cdll_get(list, i, &node);
        entry = container_of(node, struct cdll_entry, node);
        printf("list[%zd] : %d\n", i, entry->data);
    }

    /* 리스트 뒤집기 */
    printf("\nReversing List...\n");
    cdll_reverse(list);

    /* 리스트 출력 */
    i = 0;
    cdll_forward(list, cdll_print_entry, &i);

    /* 리스트 출력 */
    printf("\nNegative Index...\n");
    count = cdll_count(list);
    for (i = 1; i <= count; i++) {
        cdll_get(list, -i, &node);
        entry = container_of(node, struct cdll_entry, node);
        printf("list[%zd] : %d\n", -i, entry->data);
    }

    /* 모든 노드를 메모리에서 제거 */
    printf("\nDestroyihng List...\n");
    while (!cdll_is_empty(list)) {
        cdll_remove_tail(list, &node);
        free(container_of(node, struct cdll_entry, node));
    }

    return 0;
}
