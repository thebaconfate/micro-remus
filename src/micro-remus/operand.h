#ifndef REMUS_OPERAND_H
#define REMUS_OPERAND_H

#include "location.h"
#include "types.h"

struct Remus;

typedef enum { OPERAND_VALUE, OPERAND_LOCATION } OperandTag;

typedef struct Operand {
  OperandTag tag;
  union {
    Value value;
    Location location;
  };
} Operand;

Value *operand_fetch(Operand *operand, DeploymentId deployment_id,
                     struct Remus *remus);
#endif
