#ifndef REMUS_PROGRAM_H
#define REMUS_PROGRAM_H

#include "reactors.h"

typedef struct Program {
  Reactors reactors;
} Program;

Program program_new(Reactors reactors);

Reactors program_get_reactors(Program *program);

#endif
