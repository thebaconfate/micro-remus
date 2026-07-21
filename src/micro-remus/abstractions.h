#ifndef REMUS_TYPES_H
#define REMUS_TYPES_H

#include "option.h"
#include <stddef.h>

typedef struct Deployment Deployment;
typedef struct Command Command;
typedef struct Reactor Reactor;

typedef struct {
  Option *data;
  size_t len;
  size_t capacity;
} Inputs;

typedef struct {
  Option *data;
  size_t len;
  size_t capacity;
} Outputs;

#endif
