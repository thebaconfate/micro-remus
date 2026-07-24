#include "reactor.h"
#include "command.h"
#include <stddef.h>

Reactor reactor_new(Commands deployment_commands, Commands reaction_commands,
                    size_t input_size, size_t output_size) {
  return (Reactor){.deployment_commands = deployment_commands,
                   .reaction_commands = reaction_commands,
                   .input_size = input_size,
                   .output_size = output_size};
}

size_t reactor_input_size(Reactor *reactor) { return reactor->input_size; }

size_t reactor_output_size(Reactor *reactor) { return reactor->output_size; }

size_t reactor_deployment_size(Reactor *reactor) {
  return reactor->deployment_commands.len;
}

Command reactor_get_deployment_command(Reactor *reactor, size_t pc) {
  return reactor->deployment_commands.storage[pc];
}

size_t reactor_size(Reactor *reactor) { return reactor->reaction_commands.len; }

Command reactor_get_reaction_command(Reactor *reactor, size_t pc) {
  return reactor->reaction_commands.storage[pc];
}
