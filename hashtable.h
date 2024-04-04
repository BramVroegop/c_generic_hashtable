#include <stddef.h>

typedef struct
{
    void *key;
    void *value;
} Pair;

typedef struct
{
    Pair *values;
    size_t capacity;
    size_t length;
    int (*hash)(void *);
    int (*equals)(void *, void *, int);
} HashTable;

HashTable *create_hashtable(int (*hash)(void *),
                            int (*equals)(void *, void *, int),
                            size_t initial_capacity);
void free_hashtable(HashTable *table);
void hashtable_add(HashTable *table, void *key, void *value);
void *hashtable_get(HashTable *table, void *key, int key_size);
void *hashtable_pop(HashTable *table, void *key, int key_size);
