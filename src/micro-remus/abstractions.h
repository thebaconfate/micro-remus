#ifndef REMUS_ABSTRACTIONS_H
#define REMUS_ABSTRACTIONS_H

#include "../include/uthash.h"
#include "command.h"
#include "option.h"
#include "types.h"
#include <stddef.h>

typedef struct {
  ValueOption *storage;
  size_t len;
  size_t capacity;
} ValueOptionArray;

typedef ValueOptionArray Inputs;
typedef ValueOptionArray Outputs;

typedef struct {
  Command *storage;
  size_t len;
  size_t capacity;
} Commands;

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

typedef struct {
  DeploymentId *storage;
  size_t len;
  size_t capacity;
} Scope;

#endif
