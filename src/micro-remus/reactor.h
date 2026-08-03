#ifndef REMUS_REACTOR_H
#define REMUS_REACTOR_H

#include "abstractions.h"
#include <stddef.h>

typedef struct Reactor {
  Instructions deployment_instructions;
  Instructions reaction_instructions;
  size_t input_size;
  size_t output_size;
} Reactor;

Reactor reactor_new(Instructions deployment_instructions,
                    Instructions reaction_instructions, size_t input_size,
                    size_t output_size);

size_t reactor_input_size(Reactor *reactor);

size_t reactor_output_size(Reactor *reactor);

size_t reactor_deployment_size(Reactor *reactor);

Instruction reactor_get_deployment_instruction(Reactor *reactor, size_t pc);

size_t reactor_reactor_size(Reactor *reactor);

Instruction reactor_get_reaction_instruction(Reactor *reactor, size_t pc);

#endif
