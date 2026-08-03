#ifndef REMUS_INSTRUCTION_H
#define REMUS_INSTRUCTION_H

#include "location.h"
#include "operand.h"
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
} InstructionTag;

typedef struct AllocMono {
  Name name;
} AllocMono;

typedef struct DefRho {
  Name name;
} DefRho;

typedef struct AllocRho {
  Location location;
} AllocRho;

typedef struct Trampoline {
  Value value;
} Trampoline;

typedef struct Supply {
  Operand operand;
  Location location;
  Number number;
} Supply;

typedef struct Update {
  Operand operand;
  Location location;
} Update;

typedef struct Scan {
  Operand operand;
  Number number;
} Scan;

typedef struct React {
  Location location;
} React;

typedef struct Consume {
  Location location;
  Number number;
} Consume;

typedef struct Read {
  Location location;
} Read;

typedef struct Global {
  Name name;
} Global;

typedef struct Sink {
  Operand operand;
  Number number;
} Sink;

typedef struct MakePoly {
} MakePoly;

typedef struct AllocPoly {
  Operand operand;
  Location location;
} AllocPoly;

typedef struct Primitive {
  Name name;
} Primitive;

typedef struct {
  InstructionTag tag;
  union {
    AllocMono alloc_mono;
    DefRho def_rho;
    AllocRho alloc_rho;
    Trampoline trampoline;
    Supply supply;
    Update update;
    Scan scan;
    React react;
    Consume consume;
    Read read;
    Global global;
    Sink sink;
    MakePoly make_poly;
    AllocPoly alloc_poly;
    Primitive primitive;
  } as;
} Instruction;

void instruction_execute(Instruction *instruction, DeploymentId deployment_id,
                         struct Remus *remus);

#endif
