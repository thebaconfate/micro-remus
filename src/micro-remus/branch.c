#include "branch.h"
#include "../include/uthash.h"
#include "types.h"
#include "value.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

Branch *branches = NULL;

/**
 * @brief Frees all memory associated with a branch.
 *
 * Releases the memory allocated for stored keys, values, the branch storage,
 * and finally the branch itself.
 *
 */
void free_branches() {
  Branch *branch, *tmp;
  HASH_ITER(hh, branches, branch, tmp) {
    HASH_DEL(branches, branch);
    free(branch);
  }
}

/**
 * @brief Finds a value stored under a given name.
 *
 * Searches the branch storage for an entry matching the provided reactor name.
 *
 * @param branch The branch to search.
 * @param reactor_name The name of the value to find.
 *
 * @return A pointer to the stored value if found, or NULL if the branch,
 *         name, or matching entry does not exist.
 */
Branch *find_branch(const Name reactor_name) {
  Branch *s = NULL;
  HASH_FIND(hh, branches, reactor_name, strlen(reactor_name), s);
  return s;
}

/**
 * @brief Stores a value under a given name in a branch.
 *
 * If an entry with the given reactor name already exists, its value is updated.
 * Otherwise, a new entry is allocated and appended to the branch storage.
 *
 * @param branch The branch in which to store the value.
 * @param reactor_name The name used to identify the stored value.
 * @param value The value to store.
 *
 * @note If memory allocation fails, the value will not be stored.
 */
void add_branch(Name reactor_name, Value value) {
  Branch *s = malloc(sizeof(Branch));
  size_t name_len = strlen(reactor_name);

  s->key = malloc(name_len + 1);
  strcpy(s->key, reactor_name);

  s->val = value;

  HASH_ADD_KEYPTR(hh, branches, s->key, name_len, s);
}
