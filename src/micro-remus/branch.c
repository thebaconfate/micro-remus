#include "branch.h"
#include "../include/uthash.h"
#include "types.h"
#include "value.h"
#include <stddef.h>
#include <stdlib.h>

Branch branch_new() { return NULL; }

/**
 * @brief Frees all memory associated with the global branches hash table.
 *
 * Iterates through the global hash table, removes each entry, frees its
 * dynamically allocated key, and releases the BranchEntry structure.
 */
void branch_free(Branch *branch) {
  if (branch == NULL || *branch == NULL)
    return;
  BranchEntry *current, *tmp;
  HASH_ITER(hh, *branch, current, tmp) {
    HASH_DEL(*branch, current);
    if (current->key) {
      free(current->key);
      current->key = NULL;
    }
    free(current);
  }
  *branch = NULL;
}

/**
 * @brief Finds a stored value given a reactor name.
 *
 * Searches the global branch storage for an entry matching the provided reactor
 * name.
 *
 * @param reactor_name The key used to locate the branch entry.
 *
 * @return A pointer to the stored Value if found, or NULL if no matching entry
 * exists.
 */
ValueOption branch_find(const Branch branch, const Name reactor_name) {
  if (branch == NULL || reactor_name == NULL)
    return NULL;
  BranchEntry *s = NULL;
  HASH_FIND(hh, branch, reactor_name, strlen(reactor_name), s);
  if (s == NULL)
    return NULL;
  else
    return &s->val;
}

/**
 * @brief Stores a value under a given reactor name in the global branch
 * storage.
 *
 * Dynamically allocates a new BranchEntry, duplicates the reactor_name string
 * onto the heap, assigns the provided value, and inserts the entry into the
 * global hash table.
 *
 * @param reactor_name The string key identifying the branch entry.
 * @param value The value to store in the branch.
 *
 * @note If memory allocation fails, the entry will not be stored.
 */
void branch_add(Branch *branch, Name reactor_name, Value value) {
  BranchEntry *s = NULL;
  size_t react_name_len = strlen(reactor_name);
  HASH_FIND(hh, *branch, reactor_name, react_name_len, s);
  if (s == NULL) {
    s = (BranchEntry *)malloc(sizeof(BranchEntry));
    if (!s)
      return;
    s->key = malloc(react_name_len + 1);
    if (!s->key) {
      free(s);
      return;
    }
    strcpy(s->key, reactor_name);
    HASH_ADD_KEYPTR(hh, *branch, s->key, react_name_len, s);
  }
  s->val = value;
}
