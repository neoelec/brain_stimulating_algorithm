#include <stdio.h>
#include <stdlib.h>

#include <rcn/sgll.h>

struct sgll_entry {
    struct sgll_node node;
    int data;
};

static struct sgll_entry *sgll_create_entry(int data)
{
    struct sgll_entry *entry;

    entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate memory for list entry\n");
        exit(EXIT_FAILURE);
    }

    entry->data = data;

    return entry;
}

static void sgll_print_entry(struct sgll_node *node, void *private)
{
    size_t *idx = private;
    struct sgll_entry *entry = container_of(node, struct sgll_entry, node);

    printf("list[%zu] : %d\n", (*idx)++, entry->data);
}

int main(void)
{
    size_t i;
    struct sgll __list, *list = &__list;
    struct sgll_node *node;
    struct sgll_entry *entry;

    sgll_initialize(list);

    /* 노드 5개 추가 */
    for (i = 0; i < 5; i++) {
        entry = sgll_create_entry(i);
        sgll_insert_tail(list, &entry->node);
    }

    entry = sgll_create_entry(-1);
    sgll_insert_head(list, &entry->node);

    entry = sgll_create_entry(-2);
    sgll_insert_head(list, &entry->node);

    /* 리스트 출력 */
    i = 0;
    sgll_forward(list, sgll_print_entry, &i);

    /* 리스트의 세 번째 노드 뒤에 새 노드 삽입 */
    printf("\nInserting 3000 After [2]...\n\n");
    entry = sgll_create_entry(3000);
    sgll_insert(list, 3, &entry->node);

    /* 리스트 출력 */
    i = 0;
    sgll_forward(list, sgll_print_entry, &i);

    /* 리스트 뒤집기 */
    printf("\nReversing List...\n");
    sgll_reverse(list);

    /* 리스트 출력 */
    i = 0;
    sgll_forward(list, sgll_print_entry, &i);

    /* 모든 노드를 메모리에서 제거 */
    printf("\nDestroyihng List...\n");
    while (!sgll_is_empty(list)) {
        sgll_remove_head(list, &node);
        free(container_of(node, struct sgll_entry, node));
        sgll_remove_tail(list, &node);
    }

    return 0;
}
