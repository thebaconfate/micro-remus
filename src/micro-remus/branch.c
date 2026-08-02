#include "branch.h"
#include "hash_table.h"
#include "types.h"
#include "value.h"
#include <stddef.h>

Branch branch_new() {
  HashTable branching_point = ht_new();
  return (Branch){branching_point};
}

static void branch_free(Branch *branch) {
  ht_free(&branch->branching_points, free);
}

ValueOption branch_find(Branch *branch, const Name reactor_name) {
  return (ValueOption)ht_get(branch->branching_points, reactor_name,
                             strlen(reactor_name));
}

void branch_store(Branch *branch, Name reactor_name, Value value) {
  ValueOption val = malloc(sizeof(Value));
  *val = value;
  ht_put(&branch->branching_points, reactor_name, strlen(reactor_name),
         (void *)val, free);
}
