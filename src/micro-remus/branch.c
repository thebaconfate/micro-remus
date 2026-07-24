#include "branch.h"
#include "../include/uthash.h"
#include "types.h"
#include "value.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

Branch *branches = NULL;

/**
 * @brief Frees all memory associated with the global branches hash table.
 *
 * Iterates through the global hash table, removes each entry, frees its
 * dynamically allocated key, value pointer (if allocated), and finally releases
 * the Branch structure.
 */
void free_branches() {
  Branch *branch, *tmp;
  HASH_ITER(hh, branches, branch, tmp) {
    HASH_DEL(branches, branch);
    if (branch->key)
      free(branch->key);
    free(branch);
  }
}

/**
 * @brief Finds a branch entry stored under a given reactor name.
 *
 * Searches the global branch storage for an entry matching the provided reactor
 * name.
 *
 * @param reactor_name The key used to locate the branch entry.
 *
 * @return A pointer to the matching Branch struct if found, or NULL if
 * reactor_name is NULL or no matching entry exists.
 */
Branch *find_branch(const Name reactor_name) {
  Branch *s = NULL;
  HASH_FIND(hh, branches, reactor_name, strlen(reactor_name), s);
  return s;
}

/**
 * @brief Stores a value under a given reactor name in the global branch
 * storage.
 *
 * Dynamically allocates a new Branch entry, duplicates the reactor_name string
 * onto the heap, assigns the provided value, and inserts the entry into the
 * global hash table.
 *
 * @param reactor_name The string key identifying the branch entry.
 * @param value The value to store in the branch.
 *
 * @note If memory allocation fails, the entry will not be stored.
 */
void add_branch(Name reactor_name, Value value) {
  Branch *s = malloc(sizeof(Branch));
  size_t name_len = strlen(reactor_name);

  s->key = malloc(name_len + 1);
  strcpy(s->key, reactor_name);

  s->val = value;

  HASH_ADD_KEYPTR(hh, branches, s->key, name_len, s);
}
