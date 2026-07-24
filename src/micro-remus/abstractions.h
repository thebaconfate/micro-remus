#ifndef REMUS_ABSTRACTIONS_H
#define REMUS_ABSTRACTIONS_H

#include "../include/uthash.h"
#include "command.h"
#include "option.h"
#include "types.h"
#include <stddef.h>

struct Deployment;
struct Reactor;

typedef struct {
  Option *storage;
  size_t len;
  size_t capacity;
} OptionArray;

typedef OptionArray Inputs;
typedef OptionArray Outputs;

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

typedef OptionArray Memory;

typedef struct {
  DeploymentId *storage;
  size_t len;
  size_t capacity;
} Scope;

#endif
