#include "branch.h"
#include "value.h"
#include <stdlib.h>
#include <string.h>

Branch *branch_new(void) {
  Branch *b = malloc(sizeof(Branch));
  if (!b)
    return NULL;
  b->storage = NULL;
  b->capacity = 0;
  b->count = 0;
  return b;
}

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
