#ifndef REMUS_COMMAND_H
#define REMUS_COMMAND_H

#include "location.h"
#include "types.h"
#include "value.h"

typedef enum {
  CMD_ALLOC_MONO,
  CMD_DEF_RHO,
  CMD_ALLOC_RHO,
  CMD_TRAMPOLINE,
  CMD_SUPPLY,
  CMD_UPDATE,
  CMD_SCAN,
  CMD_REACT,
  CMD_CONSUME,
  CMD_READ,
  CMD_GLOBAL,
  CMD_SINK,
  CMD_MAKE_POLY,
  CMD_ALLOC_POLY,
  CMD_PRIMITIVE
} CommandTag;

typedef struct {
  CommandTag tag;
  union {
    Name name;
    Location location;
    Value value;
    // TODO: finish this
  } as;
} Command;

#endif
