#include <stdio.h>
#include <stdlib.h>

#include <rcn/dbll.h>

struct dbll_entry {
    struct dbll_node node;
    int data;
};

static struct dbll_entry *dbll_create_entry(int data)
{
    struct dbll_entry *entry;

    entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate memory for list entry\n");
        exit(EXIT_FAILURE);
    }

    entry->data = data;

    return entry;
}

static void dbll_print_entry(struct dbll_node *node, void *private)
{
    size_t *idx = private;
    struct dbll_entry *entry = container_of(node, struct dbll_entry, node);

    printf("list[%zu] : %d\n", (*idx)++, entry->data);
}

int main(void)
{
    size_t i;
    struct dbll_node __list, *list = &__list;
    struct dbll_node *node;
    struct dbll_entry *entry;

    dbll_initialize(list);

    /* 노드 5개 추가 */
    for (i = 0; i < 5; i++) {
        entry = dbll_create_entry(i);
        dbll_insert_tail(list, &entry->node);
    }

    /* 리스트 출력 */
    i = 0;
    dbll_forward(list, dbll_print_entry, &i);

    /* 리스트의 세 번째 노드 뒤에 새 노드 삽입 */
    printf("\nInserting 3000 After [2]...\n\n");
    entry = dbll_create_entry(3000);
    dbll_insert(list, 3, &entry->node);

    /* 리스트 출력 */
    i = 0;
    dbll_forward(list, dbll_print_entry, &i);

    /* 리스트 뒤집기 */
    printf("\nReversing List...\n");
    dbll_reverse(list);

    /* 리스트 출력 */
    i = 0;
    dbll_forward(list, dbll_print_entry, &i);

    /* 모든 노드를 메모리에서 제거 */
    printf("\nDestroyihng List...\n");
    while (!dbll_is_empty(list)) {
        dbll_remove_head(list, &node);
        free(container_of(node, struct dbll_entry, node));
    }

    return 0;
}
