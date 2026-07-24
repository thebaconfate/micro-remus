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
 * @brief Frees all memory associated with the global branches hash table.
 *
 * Iterates through the global hash table, removes each entry, frees its
 * dynamically allocated key, value pointer (if allocated), and finally releases
 * the Branch structure.
 */
void free_branches();

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
Branch *find_branch(Name reactor_name);

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
void add_branch(Name reactor_name, Value value);

#endif
