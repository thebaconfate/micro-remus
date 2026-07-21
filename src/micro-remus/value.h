#ifndef VALUE_H
#define VALUE_H

#include "types.h"

struct Branch;

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
    struct Branch *branch;
  } as;
} Value;

#endif
