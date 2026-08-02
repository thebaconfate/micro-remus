#ifndef REMUS_BRANCH_H
#define REMUS_BRANCH_H

#include "hash_table.h"
#include "types.h"
#include "value.h"

typedef Entry BranchEntry;

typedef struct Branch {
  HashTable branching_points;
} Branch;

Branch branch_new();

ValueOption branch_find(Branch *branch, const Name reactor_name);

void branch_store(Branch *branch, const Name reactor_name, Value value);

#endif
