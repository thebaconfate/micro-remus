#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "../include/uthash.h"

typedef struct Entry {
  void *key;
  size_t key_len;
  void *val;
  UT_hash_handle hh;
} Entry;

typedef Entry *HashTable;

HashTable ht_new();

void ht_put(HashTable *hash_table, const void *key, size_t key_len, void *val,
            void (*free_val)(void *));

void *ht_get(HashTable hash_table, const void *key, size_t key_len);

void ht_free(HashTable *table, void (*free_val)(void *));

#endif
