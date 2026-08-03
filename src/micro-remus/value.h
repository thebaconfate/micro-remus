#ifndef VALUE_H
#define VALUE_H

#include "types.h"

struct BranchEntry;

typedef enum {
  VAL_NUMBER,
  VAL_BOOLEAN,
  VAL_REACTOR,
  VAL_BRANCH,
} ValueType;

typedef struct Value {
  ValueType type;
  union {
    Number number;
    Boolean boolean;
    Name reactor;
    struct Branch *branch;
  } as;
} Value;

typedef Value *ValueOption;

bool value_option_equals(ValueOption a, ValueOption b);

void print_value(const Value *v);

bool value_equals(Value a, Value b);

Value value_add(Value a, Value b);

Value value_mul(Value a, Value b);

Value value_sum(const Value *values, size_t count);

Value value_product(const Value *values, size_t count);

const char *value_type_to_string(ValueType type);

#endif
