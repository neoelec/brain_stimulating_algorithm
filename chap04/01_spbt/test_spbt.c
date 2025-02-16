#include <stdio.h>
#include <stdlib.h>

#include <rcn/spbt.h>

struct spbt_entry {
    struct spbt_node node;
    char data;
};

static struct spbt_entry *spbt_create_entry(char data)
{
    struct spbt_entry *entry;

    entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate memory for tree entry\n");
        exit(EXIT_FAILURE);
    }

    entry->data = data;
    entry->node.left = NULL;
    entry->node.right = NULL;

    return entry;
}

static void spbt_destroy_entry(struct spbt_node *node)
{
    struct spbt_entry *entry;

    if (node == NULL)
        return;

    entry = container_of(node, struct spbt_entry, node);

    spbt_destroy_entry(node->left);
    spbt_destroy_entry(node->right);

    free(entry);
}

static void spbt_print_entry(struct spbt_node *node, void *private)
{
    struct spbt_entry *entry = container_of(node, struct spbt_entry, node);

    printf(" %c", entry->data);
}

static struct spbt *spbt_create_tree(void)
{
    struct spbt *tree;

    tree = malloc(sizeof(*tree));
    if (tree == NULL) {
        fprintf(stderr, "Failed to allocate memory for tree\n");
        exit(EXIT_FAILURE);
    }

    spbt_initialize(tree);

    return tree;
}

static void spbt_destroy_tree(struct spbt *tree)
{
    spbt_destroy_entry(tree->root);
    free(tree);
}

int main(void)
{
    struct spbt *tree;
    struct spbt_entry *A, *B, *C, *D, *E, *F, *G;

    tree = spbt_create_tree();

    A = spbt_create_entry('A');
    B = spbt_create_entry('B');
    C = spbt_create_entry('C');
    D = spbt_create_entry('D');
    E = spbt_create_entry('E');
    F = spbt_create_entry('F');
    G = spbt_create_entry('G');

    tree->root = &A->node;

    A->node.left = &B->node;
    B->node.left = &C->node;
    B->node.right = &D->node;

    A->node.right = &E->node;
    E->node.left = &F->node;
    E->node.right = &G->node;

    printf("Preorder ...\n");
    spbt_preorder(tree, spbt_print_entry, NULL);
    printf("\n\n");

    printf("Inorder ...\n");
    spbt_inorder(tree, spbt_print_entry, NULL);
    printf("\n\n");

    printf("Postorder ...\n");
    spbt_postorder(tree, spbt_print_entry, NULL);
    printf("\n\n");

    spbt_destroy_tree(tree);

    return 0;
}
