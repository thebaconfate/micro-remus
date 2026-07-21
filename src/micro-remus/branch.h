#ifndef REMUS_BRANCH_H
#define REMUS_BRANCH_H

#include "types.h"
#include "value.h"

typedef struct {
  Name key;
  Value *val;
} BranchEntry;

typedef struct Branch {
  BranchEntry *storage;
  size_t capacity;
  size_t count;
} Branch;

Branch *branch_new(void);
void branch_free(Branch *branch);
Value *branch_find(const Branch *branch, Name reactor_name);
void branch_store(Branch *branch, Name reactor_name, Value value);

#endif
