#include "branch.h"
#include "value.h"
#include <string.h>

/*
 * This implementation is probably optimizable by using an actual hashmap
 * implementation instead of a linear array.
 * Each find and insert is O(n), which is suboptimal when hashmaps are O(1)
 * */

/**
 * @brief Creates a new branch
 *
 * Allocates memory for a branch and initializes its storage.
 *
 * @return a pointer to the newly created branch, or NULL on failure
 */
Branch *branch_new(void) {
  Branch *b = malloc(sizeof(Branch));
  if (!b)
    return NULL;
  b->storage = NULL;
  b->capacity = 0;
  b->count = 0;
  return b;
}

/**
 * @brief Frees all memory associated with a branch.
 *
 * Releases the memory allocated for stored keys, values, the branch storage,
 * and finally the branch itself.
 *
 * @param branch The branch to free. If NULL, the function does nothing.
 */
void branch_free(Branch *branch) {
  if (!branch)
    return;
  for (size_t i = 0; i < branch->count; i++) {
    free(branch->storage[i].key);
    free(branch->storage[i].val);
  }
  free(branch->storage);
  free(branch);
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
Value *branch_find(const Branch *branch, Name reactor_name) {
  if (!branch || !reactor_name)
    return NULL;
  for (size_t i = 0; i < branch->count; i++) {
    if (strcmp(branch->storage[i].key, reactor_name) == 0) {
      return branch->storage[i].val;
    }
  }
  return NULL;
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
void branch_store(Branch *branch, Name reactor_name, Value value) {
  if (!branch)
    return;

  for (size_t i = 0; i < branch->count; i++) {
    if (strcmp(branch->storage[i].key, reactor_name) == 0) {
      *branch->storage[i].val = value;
      return;
    }
  }

  if (branch->count >= branch->capacity) {
    size_t new_capacity = branch->capacity == 0 ? 4 : branch->capacity * 2;
    BranchEntry *new_storage =
        realloc(branch->storage, new_capacity * sizeof(BranchEntry));
    if (!new_storage)
      return;
    branch->storage = new_storage;
    branch->capacity = new_capacity;
  }

  branch->storage[branch->count].key = reactor_name;
  branch->storage[branch->count].val = malloc(sizeof(Value));

  if (!branch->storage[branch->count].val)
    return;

  *branch->storage[branch->count].val = value;

  branch->count++;
}
