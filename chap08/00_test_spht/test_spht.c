#include <stdio.h>
#include <stdlib.h>

#include <rcn/spht.h>

struct spht_entry {
    size_t key;
    int value;
};

static size_t spht_key_hash(const struct spht *table, const void *_entry)
{
    const struct spht_entry *entry = _entry;

    return entry->key % table->nr_buckets;
}

static void spht_insert_entry(struct spht *table, size_t key, int value)
{
    struct spht_entry *entry;

    entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        fprintf(stderr, "Failed to allocate memory for table entry\n");
        exit(EXIT_FAILURE);
    }

    entry->key = key;
    entry->value = value;

    spht_insert(table, entry);
}

static int spht_search_entry(struct spht *table, size_t key)
{
    const struct spht_entry key_entry = {
        .key = key,
    };
    void *_entry;
    int err;

    err = spht_search(table, &key_entry, &_entry);
    if (err != 0) {
        fprintf(stderr, "Failed to get entry: %d\n", err);
        return err;
    }

    return ((struct spht_entry *)_entry)->value;
}

static struct spht *spht_create_table(size_t nr_buckets)
{
    struct spht *table;

    table = malloc(sizeof(*table));
    if (table == NULL) {
        fprintf(stderr, "Failed to allocate memory for table\n");
        exit(EXIT_FAILURE);
    }

    spht_initialize(table, nr_buckets, spht_key_hash);

    return table;
}

static void spht_destroy_entry(void *_entry, void *_table)
{
    spht_remove(_table, _entry, NULL);

    free(_entry);
}

static void spht_destroy_table(struct spht *table)
{
    spht_traverse(table, spht_destroy_entry, table);
    spht_terminate(table);

    free(table);
}

int main(void)
{
    struct spht *table;

    table = spht_create_table(193);

    spht_insert_entry(table, 418, 32114);
    spht_insert_entry(table, 9, 514);
    spht_insert_entry(table, 27, 8917);
    spht_insert_entry(table, 1031, 286);

    printf("Key:%-5d, Value:%d\n", 418, spht_search_entry(table, 418));
    printf("Key:%-5d, Value:%d\n", 9, spht_search_entry(table, 9));
    printf("Key:%-5d, Value:%d\n", 27, spht_search_entry(table, 27));
    printf("Key:%-5d, Value:%d\n", 1031, spht_search_entry(table, 1031));

    spht_destroy_table(table);

    return 0;
}
