#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <rcn/btre.h>

static int btre_compare(const void *_a, const void *_b)
{
    intptr_t a = (intptr_t)_a;
    intptr_t b = (intptr_t)_b;

    if (a > b)
        return 1;
    else if (a < b)
        return -1;
    else
        return 0;
}

static int btre_search_entry(struct btre *tree, intptr_t key)
{
    intptr_t data;
    int err;

    err = btre_search_i(tree, key, &data);
    if (err != 0)
        return err;

    return 0;
}

static int btre_insert_entry(struct btre *tree, intptr_t data)
{
    int err;

    err = btre_insert_i(tree, data);
    if (err != 0)
        return err;

    return 0;
}

static int btre_remove_entry(struct btre *tree, intptr_t data)
{
    int err;

    err = btre_search_entry(tree, data);
    if (err != 0)
        return err;

    btre_remove_i(tree, data, NULL);

    return 0;
}

static void btre_print_entry(void *entry, void *private)
{
    intptr_t data = (intptr_t)entry;

    printf("%ld ", data);
}

static struct btre *btre_create_tree(size_t t)
{
    struct btre *tree;

    tree = malloc(sizeof(*tree));
    if (tree == NULL) {
        fprintf(stderr, "Failed to allocate memory for tree\n");
        exit(EXIT_FAILURE);
    }

    btre_initialize(tree, t, btre_compare);

    return tree;
}

static void btre_destroy_tree(struct btre *tree)
{
    void *entry;

    while (btre_minimum(tree, &entry) == 0)
        btre_remove_entry(tree, *(int *)entry);

    free(tree);
}

static void btre_insert_entries_randomly(struct btre *tree)
{
    size_t i = 0;

    while (i < 100) {
        if (btre_insert_entry(tree, rand() % 500))
            i++;
    }
}

static void btre_remove_entries_randomly(struct btre *tree)
{
    while (!btre_is_empty(tree))
        btre_remove_entry(tree, rand() % 500);
}

static void btre_insert_remove99(struct btre *tree)
{
    size_t i;

    for (i = 0; i < 100; i++) {
        printf("[%.3zu]", i);
        btre_insert_entries_randomly(tree);
        printf(" - I->%s", btre_is_empty(tree) ? "FAIL" : "PASS");
        btre_remove_entries_randomly(tree);
        printf(" / R->%s\n", !btre_is_empty(tree) ? "FAIL" : "PASS");
    }
}

int main(void)
{
    struct btre *tree;

    tree = btre_create_tree(rand() % 15 + 2);

    srand(time(NULL));

    while (true) {
        unsigned int cmd;
        intptr_t data;
        int err;

        printf("\n\n------- B-TREE --------\n");
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
            scanf("%ld", &data);
        }

        switch (cmd) {
        case 1:
            btre_insert_entry(tree, data);
            break;

        case 2:
            err = btre_remove_entry(tree, data);
            if (err != 0)
                printf("\nNot found entry to delete: %ld", data);

            break;

        case 3:
            err = btre_search_entry(tree, data);
            if (err != 0)
                printf("\nNot found entry: %ld", data);
            else
                printf("\nFound entry: %ld", data);

            break;

        case 4:
            btre_forward(tree, btre_print_entry, NULL);
            break;

        case 5:
            btre_backward(tree, btre_print_entry, NULL);
            break;

        case 6:
            btre_inorder(tree, btre_print_entry, NULL);
            break;

        case 7:
            btre_preorder(tree, btre_print_entry, NULL);
            break;

        case 8:
            btre_postorder(tree, btre_print_entry, NULL);
            break;

        case 10:
            btre_insert_entries_randomly(tree);
            break;

        case 11:
            btre_remove_entries_randomly(tree);
            break;

        case 99:
            btre_insert_remove99(tree);

            break;

        default:
            printf("\n\tInvalid Choice\n");
        }
    }

    btre_destroy_tree(tree);

    return 0;
}
