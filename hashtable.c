#include <stdlib.h>
#include <stdio.h>
#include "hashtable.h"

HashTable *create_hashtable(int (*hash)(void *),
                            int (*equals)(void *, void *, int),
                            size_t initial_capacity)
{
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));

    if (table == NULL)
    {
        return NULL;
    }

    table->values = calloc(initial_capacity, sizeof(Pair));

    if (table->values == NULL)
    {
        free(table);
        return NULL;
    }

    table->hash = hash;
    table->equals = equals;
    table->capacity = initial_capacity;
    table->length = 0;

    return table;
}

void free_hashtable(HashTable *table)
{
    for (int i = 0; i < table->capacity; i++)
    {
        if (table->values[i].key != NULL)
        {
            free(table->values[i].key);
        }

        if (table->values[i].value != NULL)
        {
            free(table->values[i].value);
        }
    }

    free(table->values);
    free(table);
}

void hashtable_add(HashTable *table, void *key, void *value)
{
    Pair pair;
    pair.key = key;
    pair.value = value;

    int hash = table->hash(key) % table->capacity;

    int offset = 0;
    while (hash + offset < table->capacity && table->values[hash + offset].value != NULL)
    {
        offset++;
    }

    if (hash + offset >= table->capacity)
    {
        size_t new_capacity = table->capacity * 2;
        table->values = (Pair *)realloc(table->values, sizeof(Pair) * new_capacity);

        if (table->values == NULL)
        {
            return;
        }
        for (int i = hash + offset + 1; i < new_capacity; i++)
        {
            table->values[i].key = NULL;
            table->values[i].value = NULL;
        }

        table->capacity = new_capacity;
        table->length++;
        table->values[hash + offset + 1] = pair;
        return;
    }
    table->length++;
    table->values[hash + offset] = pair;
}

Pair *hashtable_get_pair(HashTable *table, void *key, int key_size)
{
    int hash = table->hash(key) % table->capacity;

    int offset = 0;
    while (hash + offset < table->capacity &&
           !table->equals(table->values[hash + offset].key, key, key_size))
    {
        offset++;
    }

    if (hash + offset >= table->capacity && !table->equals(table->values[hash + offset].key, key, key_size))
    {
        return NULL;
    }

    return &table->values[hash + offset];
}

void *hashtable_get(HashTable *table, void *key, int key_size)
{
    Pair *p = hashtable_get_pair(table, key, key_size);
    if (p == NULL)
    {
        return NULL;
    }

    return p->value;
}

void *hashtable_pop(HashTable *table, void *key, int key_size)
{
    Pair *p = hashtable_get_pair(table, key, key_size);
    if (p == NULL)
    {
        return NULL;
    }

    void *res = p->value;

    p->value = NULL;
    p->key = NULL;

    table->length--;

    return res;
}