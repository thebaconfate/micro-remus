#ifndef VALUE_H
#define VALUE_H

#include "types.h"

struct BranchEntry;

typedef enum {
  NUMBER,
  BOOLEAN,
  REACTOR,
  BRANCH,
} ValueType;

typedef struct Value {
  ValueType type;
  union {
    Number number;
    Boolean boolean;
    Name reactor;
    struct BranchEntry *branch;
  } as;
} Value;

void print_value(const Value *v);
bool value_equals(Value a, Value b);
Value value_add(Value a, Value b);
Value value_mul(Value a, Value b);
Value value_sum(const Value *values, size_t count);
Value value_product(const Value *values, size_t count);

#endif
