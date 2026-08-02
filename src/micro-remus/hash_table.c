
#include "hash_table.h"

HashTable ht_new() { return NULL; }

void ht_put(HashTable *hash_table, const void *key, size_t key_len, void *val,
            void (*free_val)(void *)) {
  Entry *entry = NULL;

  HASH_FIND(hh, *hash_table, key, key_len, entry);

  if (entry == NULL) {
    entry = (Entry *)malloc(sizeof(Entry));
    entry->key = malloc(key_len);
    memcpy(entry->key, key, key_len);
    entry->key_len = key_len;
    entry->val = val;
    HASH_ADD_KEYPTR(hh, *hash_table, entry->key, entry->key_len, entry);
  } else {
    free(entry->val);
    entry->val = val;
  }
}

void *ht_get(HashTable hash_table, const void *key, size_t key_len) {
  Entry *entry = NULL;
  HASH_FIND(hh, hash_table, key, key_len, entry);
  return entry->val;
}

void ht_free(HashTable *table, void (*free_val)(void *)) {
  Entry *current_entry, *tmp;
  HASH_ITER(hh, *table, current_entry, tmp) {
    HASH_DEL(*table, current_entry);
    free(current_entry->key);
    if (free_val && current_entry->val) {
      free_val(current_entry->val);
    }
    free(current_entry);
  }
  *table = NULL;
}
