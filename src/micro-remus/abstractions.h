#ifndef REMUS_ABSTRACTIONS_H
#define REMUS_ABSTRACTIONS_H

#include "../include/uthash.h"
#include "instruction.h"
#include "option.h"
#include "types.h"
#include <stddef.h>

typedef struct ValueOptionArray {
  ValueOption *storage;
  size_t len;
  size_t capacity;
} ValueOptionArray;

typedef ValueOptionArray Inputs;
typedef ValueOptionArray Outputs;

typedef struct Instructions {
  Instruction *storage;
  size_t len;
  size_t capacity;
} Instructions;

typedef struct ReactorEntry {
  Name key;
  struct Reactor *val;
  UT_hash_handle hh;
} ReactorEntry;

typedef ReactorEntry *Reactors;

typedef struct DeploymentEntry {
  DeploymentId key;
  struct Deployment *val;
  UT_hash_handle hh;
} DeploymentEntry;

typedef DeploymentEntry *Deployments;

typedef struct SignalEntry {
  Name key;
  struct Value *val;
  UT_hash_handle hh;
} SignalEntry;

typedef SignalEntry *SignalEnvironment;

typedef ValueOptionArray Memory;

typedef struct Scope {
  DeploymentId *storage;
  size_t len;
  size_t capacity;
} Scope;

ValueOptionArray value_option_array_new(size_t size);

void value_option_array_free(ValueOptionArray *array);

#endif
