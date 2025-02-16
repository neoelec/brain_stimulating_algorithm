#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#include <rcn/avlt.h>

struct avlt_entry {
    struct avlt_node node;
    int data;
};

static int avlt_compare(const struct avlt_node *_a, const struct avlt_node *_b)
{
    const struct avlt_entry *a = container_of(_a, struct avlt_entry, node);
    const struct avlt_entry *b = container_of(_b, struct avlt_entry, node);

    return a->data - b->data;
}

static struct avlt_entry *avlt_search_entry(struct avlt *tree, int data)
{
    struct avlt_entry key_entry = {
        .data = data,
    };
    struct avlt_node *node;
    int err;

    err = avlt_search(tree, &(key_entry.node), &node);
    if (err != 0)
        return NULL;

    return container_of(node, struct avlt_entry, node);
}

static int avlt_insert_entry(struct avlt *tree, int data)
{
    struct avlt_entry *entry;
    int err;

    entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate memory for tree entry\n");
        exit(EXIT_FAILURE);
    }

    entry->data = data;

    err = avlt_insert(tree, &entry->node);
    if (err != 0) {
        free(entry);
        return err;
    }

    return 0;
}

static int avlt_remove_entry(struct avlt *tree, int data)
{
    struct avlt_entry *entry;

    entry = avlt_search_entry(tree, data);

    if (entry == NULL)
        return -ENOENT;

    avlt_remove(tree, &entry->node);
    free(entry);

    return 0;
}

static void avlt_print_entry(struct avlt_node *node, void *private)
{
    struct avlt_entry *entry = container_of(node, struct avlt_entry, node);

    printf("%d ", entry->data);
}

static struct avlt *avlt_create_tree(void)
{
    struct avlt *tree;

    tree = malloc(sizeof(*tree));
    if (tree == NULL) {
        fprintf(stderr, "Failed to allocate memory for tree\n");
        exit(EXIT_FAILURE);
    }

    avlt_initialize(tree, avlt_compare);

    return tree;
}

static void avlt_destroy_tree(struct avlt *tree)
{
    struct avlt_node *node;

    while (avlt_minimum(tree, &node) == 0) {
        struct avlt_entry *entry = container_of(node, struct avlt_entry, node);

        avlt_remove_entry(tree, entry->data);
    }

    free(tree);
}

static void avlt_insert_entries_randomly(struct avlt *tree)
{
    size_t i = 0;

    while (i < 100) {
        if (avlt_insert_entry(tree, rand() % 500) == 0)
            i++;
    }
}

static void avlt_remove_entries_randomly(struct avlt *tree)
{
    while (!avlt_is_empty(tree))
        avlt_remove_entry(tree, rand() % 500);
}

static void avlt_insert_remove99(struct avlt *tree)
{
    size_t i;

    for (i = 0; i < 100; i++) {
        printf("[%.3zu]", i);
        avlt_insert_entries_randomly(tree);
        printf(" - I->%s", avlt_is_empty(tree) ? "FAIL" : "PASS");
        avlt_remove_entries_randomly(tree);
        printf(" / R->%s\n", !avlt_is_empty(tree) ? "FAIL" : "PASS");
    }
}

int main(void)
{
    struct avlt *tree;

    tree = avlt_create_tree();

    srand(time(NULL));

    while (true) {
        unsigned int cmd;
        int data;
        struct avlt_entry *entry;
        int err;

        printf("\n\n------- avlt TREE --------\n");
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
            avlt_insert_entry(tree, data);
            break;

        case 2:
            err = avlt_remove_entry(tree, data);
            if (err != 0)
                printf("\nNot found entry to delete: %d", data);

            break;

        case 3:
            entry = avlt_search_entry(tree, data);
            if (entry == NULL)
                printf("\nNot found entry: %d", data);
            else
                printf("\nFound entry: %d", entry->data);

            break;

        case 4:
            avlt_forward(tree, avlt_print_entry, NULL);
            break;

        case 5:
            avlt_backward(tree, avlt_print_entry, NULL);
            break;

        case 6:
            avlt_inorder(tree, avlt_print_entry, NULL);
            break;

        case 7:
            avlt_preorder(tree, avlt_print_entry, NULL);
            break;

        case 8:
            avlt_postorder(tree, avlt_print_entry, NULL);
            break;

        case 10:
            avlt_insert_entries_randomly(tree);
            break;

        case 11:
            avlt_remove_entries_randomly(tree);
            break;

        case 99:
            avlt_insert_remove99(tree);
            break;

        default:
            printf("\n\tInvalid Choice\n");
        }
    }

    avlt_destroy_tree(tree);

    return 0;
}
