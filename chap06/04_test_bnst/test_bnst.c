#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <rcn/bnst.h>

struct bnst_entry {
    struct bnst_node node;
    int data;
};

static int bnst_compare(const struct bnst_node *_a, const struct bnst_node *_b)
{
    const struct bnst_entry *a = container_of(_a, struct bnst_entry, node);
    const struct bnst_entry *b = container_of(_b, struct bnst_entry, node);

    return a->data - b->data;
}

static struct bnst_entry *bnst_search_entry(struct bnst *tree, int data)
{
    struct bnst_entry key_entry = {
        .data = data,
    };
    struct bnst_node *node;
    int err;

    err = bnst_search(tree, &(key_entry.node), &node);
    if (err != 0)
        return NULL;

    return container_of(node, struct bnst_entry, node);
}

static int bnst_insert_entry(struct bnst *tree, int data)
{
    struct bnst_entry *entry;
    int err;

    entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate memory for tree entry\n");
        exit(EXIT_FAILURE);
    }

    entry->data = data;

    err = bnst_insert(tree, &entry->node);
    if (err != 0) {
        free(entry);
        return err;
    }

    return 0;
}

static int bnst_remove_entry(struct bnst *tree, int data)
{
    struct bnst_entry *entry;

    entry = bnst_search_entry(tree, data);

    if (entry == NULL)
        return -ENOENT;

    bnst_remove(tree, &entry->node);
    free(entry);

    return 0;
}

static void bnst_print_entry(struct bnst_node *node, void *private)
{
    const struct bnst_entry *entry;

    entry = container_of(node, struct bnst_entry, node);
    printf(" %d", entry->data);
}

static struct bnst *bnst_create_tree(void)
{
    struct bnst *tree;

    tree = malloc(sizeof(*tree));
    if (tree == NULL) {
        fprintf(stderr, "Failed to allocate memory for tree\n");
        exit(EXIT_FAILURE);
    }

    bnst_initialize(tree, bnst_compare);

    return tree;
}

static void bnst_destroy_tree(struct bnst *tree)
{
    struct bnst_node *node;

    while (bnst_minimum(tree, &node) == 0) {
        struct bnst_entry *entry = container_of(node, struct bnst_entry, node);

        bnst_remove_entry(tree, entry->data);
    }

    free(tree);
}

int main(void)
{
    struct bnst *tree;

    tree = bnst_create_tree();

    /* 트리에 노드 추가 */
    bnst_insert_entry(tree, 22);
    bnst_insert_entry(tree, 9918);
    bnst_insert_entry(tree, 424);
    bnst_insert_entry(tree, 17);
    bnst_insert_entry(tree, 3);

    bnst_insert_entry(tree, 98);
    bnst_insert_entry(tree, 38);

    bnst_insert_entry(tree, 760);
    bnst_insert_entry(tree, 317);
    bnst_insert_entry(tree, 1);

    /* 트리 출력 */
    bnst_inorder(tree, bnst_print_entry, NULL);
    printf("\n");

    /* 특정 노드 삭제 */
    printf("Removing 98...\n");

    bnst_remove_entry(tree, 98);

    bnst_inorder(tree, bnst_print_entry, NULL);
    printf("\n");

    /* 새 노드 삽입 */
    printf("Inserting 111...\n");

    bnst_insert_entry(tree, 111);
    bnst_inorder(tree, bnst_print_entry, NULL);
    printf("\n");

    /* 트리 소멸시키기 */
    bnst_destroy_tree(tree);

    return 0;
}
