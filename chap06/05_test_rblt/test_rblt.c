#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#include <rcn/rblt.h>

struct rblt_entry {
    struct rblt_node node;
    int data;
};

static int rblt_compare(const struct rblt_node *_a, const struct rblt_node *_b)
{
    const struct rblt_entry *a = container_of(_a, struct rblt_entry, node);
    const struct rblt_entry *b = container_of(_b, struct rblt_entry, node);

    return a->data - b->data;
}

static struct rblt_entry *rblt_search_entry(struct rblt *tree, int data)
{
    struct rblt_entry key_entry = {
        .data = data,
    };
    struct rblt_node *node;
    int err;

    err = rblt_search(tree, &(key_entry.node), &node);
    if (err != 0)
        return NULL;

    return container_of(node, struct rblt_entry, node);
}

static int rblt_insert_entry(struct rblt *tree, int data)
{
    struct rblt_entry *entry;
    int err;

    entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate memory for tree entry\n");
        exit(EXIT_FAILURE);
    }

    entry->data = data;

    err = rblt_insert(tree, &entry->node);
    if (err != 0) {
        free(entry);
        return err;
    }

    return 0;
}

static int rblt_remove_entry(struct rblt *tree, int data)
{
    struct rblt_entry *entry;

    entry = rblt_search_entry(tree, data);

    if (entry == NULL)
        return -ENOENT;

    rblt_remove(tree, &entry->node);
    free(entry);

    return 0;
}

static void rblt_print_entry(struct rblt_node *node, void *private)
{
    struct rblt_entry *entry = container_of(node, struct rblt_entry, node);

    printf("%d ", entry->data);
}

static struct rblt *rblt_create_tree(void)
{
    struct rblt *tree;

    tree = malloc(sizeof(*tree));
    if (tree == NULL) {
        fprintf(stderr, "Failed to allocate memory for tree\n");
        exit(EXIT_FAILURE);
    }

    rblt_initialize(tree, rblt_compare);

    return tree;
}

static void rblt_destroy_tree(struct rblt *tree)
{
    struct rblt_node *node;

    while (rblt_minimum(tree, &node) == 0) {
        struct rblt_entry *entry = container_of(node, struct rblt_entry, node);

        rblt_remove_entry(tree, entry->data);
    }

    free(tree);
}

static void rblt_insert_entries_randomly(struct rblt *tree)
{
    size_t i = 0;

    while (i < 100) {
        if (rblt_insert_entry(tree, rand() % 500) == 0)
            i++;
    }
}

static void rblt_remove_entries_randomly(struct rblt *tree)
{
    while (!rblt_is_empty(tree))
        rblt_remove_entry(tree, rand() % 500);
}

static void rblt_insert_remove99(struct rblt *tree)
{
    size_t i;

    for (i = 0; i < 100; i++) {
        printf("[%.3zu]", i);
        rblt_insert_entries_randomly(tree);
        printf(" - I->%s", rblt_is_empty(tree) ? "FAIL" : "PASS");
        rblt_remove_entries_randomly(tree);
        printf(" / R->%s\n", !rblt_is_empty(tree) ? "FAIL" : "PASS");
    }
}

int main(void)
{
    struct rblt *tree;

    tree = rblt_create_tree();

    srand(time(NULL));

    while (true) {
        unsigned int cmd;
        int data;
        struct rblt_entry *entry;
        int err;

        printf("\n\n------- RED-BLACK TREE --------\n");
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
            rblt_insert_entry(tree, data);
            break;

        case 2:
            err = rblt_remove_entry(tree, data);
            if (err != 0)
                printf("\nNot found entry to delete: %d", data);

            break;

        case 3:
            entry = rblt_search_entry(tree, data);
            if (entry == NULL)
                printf("\nNot found entry: %d", data);
            else
                printf("\nFound entry: %d", entry->data);

            break;

        case 4:
            rblt_forward(tree, rblt_print_entry, NULL);
            break;

        case 5:
            rblt_backward(tree, rblt_print_entry, NULL);
            break;

        case 6:
            rblt_inorder(tree, rblt_print_entry, NULL);
            break;

        case 7:
            rblt_preorder(tree, rblt_print_entry, NULL);
            break;

        case 8:
            rblt_postorder(tree, rblt_print_entry, NULL);
            break;

        case 10:
            rblt_insert_entries_randomly(tree);
            break;

        case 11:
            rblt_remove_entries_randomly(tree);
            break;

        case 99:
            rblt_insert_remove99(tree);
            break;

        default:
            printf("\n\tInvalid Choice\n");
        }
    }

    rblt_destroy_tree(tree);

    return 0;
}
