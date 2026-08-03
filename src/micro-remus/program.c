#include "program.h"
#include "abstractions.h"
#include "instruction.h"
#include "location.h"
#include "reactor.h"
#include "reactors.h"

Program program_new(Reactors reactors) {
  Instructions deployment_instructions;
  Instructions reaction_instructions;
  Reactor reactor;
  Primitive primitive;
  Sink sink;
  Location location;

  // TODO: Implement this

  reactor = reactor_new(deployment_instructions, reaction_instructions, 2, 1);
  reactors_put(&reactors, "+", reactor);
  return (Program){.reactors = reactors};
}

Reactors program_get_reactors(Program *program) { return program->reactors; }
