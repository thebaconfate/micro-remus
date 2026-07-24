#ifndef REMUS_BRANCH_H
#define REMUS_BRANCH_H

#include "../include/uthash.h"
#include "types.h"
#include "value.h"

typedef struct Branch {
  Name key;
  Value val;
  UT_hash_handle hh;
} Branch;

/**
 * @brief Frees all memory associated with a branch.
 *
 * Releases the memory allocated for stored keys, values, the branch storage,
 * and finally the branch itself.
 *
 */
void free_branches();

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
Branch *find_branch(Name reactor_name);

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
void add_branch(Name reactor_name, Value value);

#endif
