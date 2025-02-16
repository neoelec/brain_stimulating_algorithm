#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rcn/chht.h>

struct chht_entry {
    struct dbll_node node;
    const char *key;
    const char *value;
};

static size_t chht_key_hash(const struct chht *table,
                            const struct dbll_node *node)
{
    const struct chht_entry *entry =
        container_of(node, struct chht_entry, node);
    size_t hash = 0;
    size_t i;

    for (i = 0; i < strlen(entry->key); i++)
        hash = (hash << 3) + entry->key[i];

    return hash % table->nr_buckets;
}

static int chht_compare(const struct dbll_node *_a, const struct dbll_node *_b)
{
    const struct chht_entry *a = container_of(_a, struct chht_entry, node);
    const struct chht_entry *b = container_of(_b, struct chht_entry, node);

    return strcmp(a->key, b->key);
}

static void chht_insert_entry(struct chht *table, const char *key,
                              const char *value)
{
    struct chht_entry *entry;

    entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate memory for table entry\n");
        exit(EXIT_FAILURE);
    }

    entry->key = key;
    entry->value = value;

    chht_insert(table, &entry->node);
    if (entry->node.prev != entry->node.next)
        printf("Collision occured : Key(%s), Address(%zu)\n", key,
               table->key_hash(table, &entry->node));
}

static const char *chht_search_entry(struct chht *table, const char *key)
{
    struct chht_entry key_entry = {
        .key = key,
    };
    struct dbll_node *node;
    struct chht_entry *entry;
    int err;

    err = chht_search(table, &key_entry.node, &node);
    if (err != 0)
        return "NiL";

    entry = container_of(node, struct chht_entry, node);

    return entry->value;
}

static struct chht *chht_create_table(size_t nr_buckets)
{
    struct chht *table;

    table = malloc(CHHT_BYTES(nr_buckets));
    if (table == NULL) {
        fprintf(stderr, "Failed to allocate memory for table\n");
        exit(EXIT_FAILURE);
    }

    chht_initialize(table, nr_buckets, chht_key_hash, chht_compare);

    return table;
}

static void chht_destroy_table(struct chht *table)
{
    struct dbll_node *bucket = table->bucket;
    size_t i;

    for (i = 0; i < table->nr_buckets; i++) {
        struct dbll_node *head = &bucket[i];

        while (head->next != head) {
            struct chht_entry *entry;

            entry = container_of(head->next, struct chht_entry, node);
            dbll_unlink(head->next);
            free(entry);
        }
    }

    free(table);
}

int main(void)
{
    struct chht *table;

    table = chht_create_table(12289);

    chht_insert_entry(table, "MSFT", "Microsoft Corporation");
    chht_insert_entry(table, "JAVA", "Sun Microsystems");
    chht_insert_entry(table, "REDH", "Red Hat Linux");
    chht_insert_entry(table, "APAC", "Apache Org");
    chht_insert_entry(table, "ZYMZZ", "Unisys Ops Check"); /* APAC와 충돌 */
    chht_insert_entry(table, "IBM", "IBM Ltd.");
    chht_insert_entry(table, "ORCL", "Oracle Corporation");
    chht_insert_entry(table, "CSCO", "Cisco Systems, Inc.");
    chht_insert_entry(table, "GOOG", "Google Inc.");
    chht_insert_entry(table, "YHOO", "Yahoo! Inc.");
    chht_insert_entry(table, "NOVL", "Novell, Inc.");

    printf("\n");
    printf("Key:%-6s, Value:%s\n", "MSFT", chht_search_entry(table, "MSFT"));
    printf("Key:%-6s, Value:%s\n", "REDH", chht_search_entry(table, "REDH"));
    printf("Key:%-6s, Value:%s\n", "APAC", chht_search_entry(table, "APAC"));
    printf("Key:%-6s, Value:%s\n", "ZYMZZ", chht_search_entry(table, "ZYMZZ"));
    printf("Key:%-6s, Value:%s\n", "JAVA", chht_search_entry(table, "JAVA"));
    printf("Key:%-6s, Value:%s\n", "IBM", chht_search_entry(table, "IBM"));
    printf("Key:%-6s, Value:%s\n", "ORCL", chht_search_entry(table, "ORCL"));
    printf("Key:%-6s, Value:%s\n", "CSCO", chht_search_entry(table, "CSCO"));
    printf("Key:%-6s, Value:%s\n", "GOOG", chht_search_entry(table, "GOOG"));
    printf("Key:%-6s, Value:%s\n", "YHOO", chht_search_entry(table, "YHOO"));
    printf("Key:%-6s, Value:%s\n", "NOVL", chht_search_entry(table, "NOVL"));

    chht_destroy_table(table);

    return 0;
}
