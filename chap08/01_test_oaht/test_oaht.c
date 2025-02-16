#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rcn/oaht.h>

struct oaht_entry {
    const char *key;
    const char *value;
};

static size_t oaht_key_hash(const struct oaht *table, const void *_entry)
{
    const struct oaht_entry *entry = (const struct oaht_entry *)_entry;
    size_t hash = 0;
    size_t i;

    for (i = 0; i < strlen(entry->key); i++)
        hash = (hash << 3) + entry->key[i];

    return hash % table->nr_buckets;
}

static size_t oaht_step_hash(const struct oaht *table, const void *_entry)
{
    const struct oaht_entry *entry = (const struct oaht_entry *)_entry;
    size_t hash = 0;
    size_t i;

    for (i = 0; i < strlen(entry->key); i++)
        hash = (hash << 2) + entry->key[i];

    return (hash % (table->nr_buckets - 3)) + 1;
}

static int oaht_compare(const void *_a, const void *_b)
{
    const struct oaht_entry *a = (const struct oaht_entry *)_a;
    const struct oaht_entry *b = (const struct oaht_entry *)_b;

    return (int)strcmp(a->key, b->key);
}

static void oaht_insert_entry(struct oaht *table, const char *key,
                              const char *value)
{
    struct oaht_entry *entry;

    entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate memory for table entry\n");
        exit(EXIT_FAILURE);
    }

    entry->key = key;
    entry->value = value;

    oaht_expand(table, 50);
    oaht_insert(table, entry);
}

static void oaht_remove_entry(struct oaht *table, const char *key)
{
    struct oaht_entry key_entry = {
        .key = key,
    };
    void *entry;
    ssize_t err;

    err = oaht_remove(table, &key_entry, &entry);
    if (err != 0)
        return;

    oaht_shrink(table, 50);

    free(entry);
}

static const char *oaht_search_entry(struct oaht *table, const char *key)
{
    struct oaht_entry key_entry = {
        .key = key,
    };
    void *entry;
    ssize_t err;

    err = oaht_search(table, &key_entry, &entry);
    if (err != 0)
        return "NiL";

    return ((struct oaht_entry *)entry)->value;
}

static struct oaht *oaht_create_table(size_t nr_buckets)
{
    struct oaht *table;

    table = malloc(sizeof(*table));
    if (table == NULL) {
        fprintf(stderr, "Failed to allocate memory for table\n");
        exit(EXIT_FAILURE);
    }

    oaht_initialize(table, nr_buckets, oaht_key_hash, oaht_step_hash,
                    oaht_compare);

    return table;
}

static void oaht_destroy_entry(void *_entry, void *_table)
{
    oaht_remove(_table, _entry, NULL);

    free(_entry);
}

static void oaht_destroy_table(struct oaht *table)
{
    oaht_traverse(table, oaht_destroy_entry, table);
    oaht_terminate(table);

    free(table);
}

int main(void)
{
    struct oaht *table;

    table = oaht_create_table(11);

    oaht_insert_entry(table, "MSFT", "Microsoft Corporation");
    oaht_insert_entry(table, "JAVA", "Sun Microsystems");
    oaht_insert_entry(table, "REDH", "Red Hat Linux");
    oaht_insert_entry(table, "APAC", "Apache Org");
    oaht_insert_entry(table, "ZYMZZ", "Unisys Ops Check"); /* APAC와 충돌 */
    oaht_insert_entry(table, "IBM", "IBM Ltd.");
    oaht_insert_entry(table, "ORCL", "Oracle Corporation");
    oaht_insert_entry(table, "CSCO", "Cisco Systems, Inc.");
    oaht_insert_entry(table, "GOOG", "Google Inc.");
    oaht_insert_entry(table, "YHOO", "Yahoo! Inc.");
    oaht_insert_entry(table, "NOVL", "Novell, Inc.");

    printf("\n");
    printf("Key:%-6s, Value:%s\n", "MSFT", oaht_search_entry(table, "MSFT"));
    printf("Key:%-6s, Value:%s\n", "REDH", oaht_search_entry(table, "REDH"));
    printf("Key:%-6s, Value:%s\n", "APAC", oaht_search_entry(table, "APAC"));
    printf("Key:%-6s, Value:%s\n", "ZYMZZ", oaht_search_entry(table, "ZYMZZ"));
    printf("Key:%-6s, Value:%s\n", "JAVA", oaht_search_entry(table, "JAVA"));
    printf("Key:%-6s, Value:%s\n", "IBM", oaht_search_entry(table, "IBM"));
    printf("Key:%-6s, Value:%s\n", "ORCL", oaht_search_entry(table, "ORCL"));
    printf("Key:%-6s, Value:%s\n", "CSCO", oaht_search_entry(table, "CSCO"));
    printf("Key:%-6s, Value:%s\n", "GOOG", oaht_search_entry(table, "GOOG"));
    printf("Key:%-6s, Value:%s\n", "YHOO", oaht_search_entry(table, "YHOO"));
    printf("Key:%-6s, Value:%s\n", "NOVL", oaht_search_entry(table, "NOVL"));

    oaht_remove_entry(table, "YHOO");
    oaht_remove_entry(table, "REDH");
    oaht_remove_entry(table, "APAC");
    oaht_remove_entry(table, "CSCO");
    oaht_remove_entry(table, "MSFT");
    oaht_remove_entry(table, "GOOG");
    oaht_remove_entry(table, "IBM");

    oaht_destroy_table(table);

    return 0;
}
