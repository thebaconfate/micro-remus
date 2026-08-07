#ifndef REMUS_ABSTRACTIONS_H
#define REMUS_ABSTRACTIONS_H

#include "types.h"
#include "value.h"
#include <stddef.h>

typedef struct ValueArray {
  ValueOption *storage;
  size_t len;
  size_t capacity;
} ValueArray;

typedef ValueArray Inputs;
typedef ValueArray Outputs;

typedef ValueArray Memory;

typedef struct Scope {
  DeploymentId *storage;
  size_t len;
  size_t capacity;
} Scope;

ValueArray value_option_array_new(size_t size);

void value_option_array_free(ValueArray *array);

#endif
