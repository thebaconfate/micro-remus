#include "branch.h"
#include "../include/uthash.h"
#include "types.h"
#include "value.h"
#include <stddef.h>
#include <stdlib.h>

static Branches branches = NULL;

/**
 * @brief Frees all memory associated with the global branches hash table.
 *
 * Iterates through the global hash table, removes each entry, frees its
 * dynamically allocated key, and releases the BranchEntry structure.
 */
void branch_free() {
  BranchEntry *branch, *tmp;
  HASH_ITER(hh, branches, branch, tmp) {
    HASH_DEL(branches, branch);
    if (branch->key)
      free(branch->key);
    free(branch);
  }
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
Value *branch_find(const Name reactor_name) {
  BranchEntry *s = NULL;
  HASH_FIND(hh, branches, reactor_name, strlen(reactor_name), s);
  return s ? &(s->val) : NULL;
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
void branch_add(Name reactor_name, Value value) {
  BranchEntry *s = malloc(sizeof(BranchEntry));
  if (!s)
    return;

  size_t name_len = strlen(reactor_name);

  s->key = malloc(name_len + 1);
  if (!s->key) {
    free(s);
    return;
  }
  strcpy(s->key, reactor_name);

  s->val = value;

  HASH_ADD_KEYPTR(hh, branches, s->key, name_len, s);
}
