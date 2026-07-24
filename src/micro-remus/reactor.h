
#include "abstractions.h"
#include "command.h"
#include <stddef.h>
typedef struct Reactor {
  Commands deployment_commands;
  Commands reaction_commands;
  size_t input_size;
  size_t output_size;
} Reactor;

Reactor reactor_new(Commands deployment_commands, Commands reaction_commands,
                    size_t input_size, size_t output_size);

size_t reactor_input_size(Reactor *reactor);
size_t reactor_output_size(Reactor *reactor);
size_t reactor_deployment_size(Reactor *reactor);
Command reactor_get_deployment_command(Reactor *reactor, size_t pc);
size_t reactor_size(Reactor *reactor);
Command reactor_get_reaction_command(Reactor *reactor, size_t pc);
