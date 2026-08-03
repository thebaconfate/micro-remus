#include "reactor.h"
#include "instruction.h"
#include <stddef.h>

Reactor reactor_new(Instructions deployment_instructions,
                    Instructions reaction_instructions, size_t input_size,
                    size_t output_size) {
  return (Reactor){.deployment_instructions = deployment_instructions,
                   .reaction_instructions = reaction_instructions,
                   .input_size = input_size,
                   .output_size = output_size};
}

size_t reactor_input_size(Reactor *reactor) { return reactor->input_size; }

size_t reactor_output_size(Reactor *reactor) { return reactor->output_size; }

size_t reactor_deployment_size(Reactor *reactor) {
  return reactor->deployment_instructions.len;
}

Instruction reactor_get_deployment_instruction(Reactor *reactor, size_t pc) {
  return reactor->deployment_instructions.storage[pc];
}

size_t reactor_reactor_size(Reactor *reactor) {
  return reactor->reaction_instructions.len;
}

Instruction reactor_get_reaction_instruction(Reactor *reactor, size_t pc) {
  return reactor->reaction_instructions.storage[pc];
}
