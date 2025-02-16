#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#include <rcn/splt.h>

struct splt_entry {
    struct splt_node node;
    int data;
};

static int splt_compare(const struct splt_node *_a, const struct splt_node *_b)
{
    const struct splt_entry *a = container_of(_a, struct splt_entry, node);
    const struct splt_entry *b = container_of(_b, struct splt_entry, node);

    return a->data - b->data;
}

static struct splt_entry *splt_search_entry(struct splt *tree, int data)
{
    struct splt_entry key_entry = {
        .data = data,
    };
    struct splt_node *node;
    int err;

    err = splt_search(tree, &(key_entry.node), &node);
    if (err != 0)
        return NULL;

    return container_of(node, struct splt_entry, node);
}

static int splt_insert_entry(struct splt *tree, int data)
{
    struct splt_entry *entry;
    int err;

    entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate memory for tree entry\n");
        exit(EXIT_FAILURE);
    }

    entry->data = data;

    err = splt_insert(tree, &entry->node);
    if (err != 0) {
        free(entry);
        return err;
    }

    return 0;
}

static int splt_remove_entry(struct splt *tree, int data)
{
    struct splt_entry *entry;

    entry = splt_search_entry(tree, data);

    if (entry == NULL)
        return -ENOENT;

    splt_remove(tree, &entry->node);
    free(entry);

    return 0;
}

static void splt_print_entry(struct splt_node *node, void *private)
{
    struct splt_entry *entry = container_of(node, struct splt_entry, node);

    printf("%d ", entry->data);
}

static struct splt *splt_create_tree(void)
{
    struct splt *tree;

    tree = malloc(sizeof(*tree));
    if (tree == NULL) {
        fprintf(stderr, "Failed to allocate memory for tree\n");
        exit(EXIT_FAILURE);
    }

    splt_initialize(tree, splt_compare);

    return tree;
}

static void splt_destroy_tree(struct splt *tree)
{
    struct splt_node *node;

    while (splt_minimum(tree, &node) == 0) {
        struct splt_entry *entry = container_of(node, struct splt_entry, node);

        splt_remove_entry(tree, entry->data);
    }

    free(tree);
}

static void splt_insert_entries_randomly(struct splt *tree)
{
    size_t i = 0;

    while (i < 100) {
        if (splt_insert_entry(tree, rand() % 500) == 0)
            i++;
    }
}

static void splt_remove_entries_randomly(struct splt *tree)
{
    while (!splt_is_empty(tree))
        splt_remove_entry(tree, rand() % 500);
}

static void splt_insert_remove99(struct splt *tree)
{
    size_t i;

    for (i = 0; i < 100; i++) {
        printf("[%.3zu]", i);
        splt_insert_entries_randomly(tree);
        printf(" - I->%s", splt_is_empty(tree) ? "FAIL" : "PASS");
        splt_remove_entries_randomly(tree);
        printf(" / R->%s\n", !splt_is_empty(tree) ? "FAIL" : "PASS");
    }
}

int main(void)
{
    struct splt *tree;

    tree = splt_create_tree();

    srand(time(NULL));

    while (true) {
        unsigned int cmd;
        int data;
        struct splt_entry *entry;
        int err;

        printf("\n\n------- SPLAY TREE --------\n");
        printf("\n(1) Create a node");
        printf("\n(2) Remove a node");
        printf("\n(3) Search a node");
        printf("\n(4) Print Forward");
        printf("\n(5) Print Backward");
        printf("\n(6) Print Inorder");
        printf("\n(7) Print Preorder");
        printf("\n(8) Print Postorder");
        printf("\n(10) Create 100 random nodes");
        printf("\n(11) Remove all nodes randomly");
        printf("\n(0) Quit");

        printf("\n\nEnter Your Choice: ");
        scanf("%u", &cmd);

        if (cmd == 0) {
            printf("\n\tProgram Terminated\n");
            break;
        } else if (cmd >= 1 && cmd <= 3) {
            printf("\nEnter data: ");
            scanf("%d", &data);
        }

        switch (cmd) {
        case 1:
            splt_insert_entry(tree, data);
            break;

        case 2:
            err = splt_remove_entry(tree, data);
            if (err != 0)
                printf("\nNot found entry to delete: %d", data);

            break;

        case 3:
            entry = splt_search_entry(tree, data);
            if (entry == NULL)
                printf("\nNot found entry: %d", data);
            else
                printf("\nFound entry: %d", entry->data);

            break;

        case 4:
            splt_forward(tree, splt_print_entry, NULL);
            break;

        case 5:
            splt_backward(tree, splt_print_entry, NULL);
            break;

        case 6:
            splt_inorder(tree, splt_print_entry, NULL);
            break;

        case 7:
            splt_preorder(tree, splt_print_entry, NULL);
            break;

        case 8:
            splt_postorder(tree, splt_print_entry, NULL);
            break;

        case 10:
            splt_insert_entries_randomly(tree);
            break;

        case 11:
            splt_remove_entries_randomly(tree);
            break;

        case 99:
            splt_insert_remove99(tree);
            break;

        default:
            printf("\n\tInvalid Choice\n");
        }
    }

    splt_destroy_tree(tree);

    return 0;
}
