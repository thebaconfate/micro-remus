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
    AllocPoly alloc_poly;
    Primitive primitive;
  } as;
} Instruction;

typedef struct Instructions {
  Instruction *storage;
  size_t len;
  size_t capacity;
} Instructions;

#define INSTRUCTION_LIST(X)                                                    \
  X(alloc_mono, CMD_ALLOC_MONO, AllocMono)                                     \
  X(def_rho, CMD_DEF_RHO, DefRho)                                              \
  X(alloc_rho, CMD_ALLOC_RHO, AllocRho)                                        \
  X(trampoline, CMD_TRAMPOLINE, Trampoline)                                    \
  X(supply, CMD_SUPPLY, Supply)                                                \
  X(update, CMD_UPDATE, Update)                                                \
  X(scan, CMD_SCAN, Scan)                                                      \
  X(react, CMD_REACT, React)                                                   \
  X(consume, CMD_CONSUME, Consume)                                             \
  X(read, CMD_READ, Read)                                                      \
  X(global, CMD_GLOBAL, Global)                                                \
  X(sink, CMD_SINK, Sink)                                                      \
  X(alloc_poly, CMD_ALLOC_POLY, AllocPoly)                                     \
  X(primitive, CMD_PRIMITIVE, Primitive)

#define MAKE_INSTR_CONSTRUCTOR(name, tag_enum, struct_type)                    \
  static inline Instruction inst_##name(struct_type payload) {                 \
    return (Instruction){.tag = tag_enum, .as.name = payload};                 \
  }

INSTRUCTION_LIST(MAKE_INSTR_CONSTRUCTOR);

#undef MAKE_CONSTRUCTOR
#undef INSTRUCTION_LIST

void instruction_execute(Instruction *instruction, DeploymentId deployment_id,
                         struct Remus *remus);

Instructions instructions_new();

void instructions_add(Instructions *instructions, Instruction instruction);

#endif
