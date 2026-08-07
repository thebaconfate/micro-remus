#include "program.h"
#include "instruction.h"
#include "location.h"
#include "operand.h"
#include "reactor.h"
#include "reactors.h"

static Instruction create_default_sink() {
  Location loc_r_1 = (Location){.type = LOC_R, .index = 1};
  Operand location_r_1 =
      (Operand){.tag = OPERAND_LOCATION, .as.location = loc_r_1};
  Sink sink_location_r_1 = (Sink){.operand = location_r_1, .number = 1};
  return inst_sink(sink_location_r_1);
}

static Reactor program_default_reactor_new(const Name name, size_t in,
                                           size_t out) {
  Instructions deployment_instructions = instructions_new();
  Instructions reaction_instructions = instructions_new();
  Instruction sink_instr = create_default_sink();
  instructions_add(&reaction_instructions,
                   inst_primitive((Primitive){.name = name}));
  instructions_add(&reaction_instructions, sink_instr);
  return reactor_new(deployment_instructions, reaction_instructions, in, out);
}

static void init_plus_reactor(Reactors *reactors) {
  Reactor reactor = program_default_reactor_new("+", 2, 1);
  reactors_put(reactors, "+", reactor);
}

static void init_minus_reactor(Reactors *reactors) {
  Reactor reactor = program_default_reactor_new("-", 2, 1);
  reactors_put(reactors, "-", reactor);
}

static void init_mul_reactor(Reactors *reactors) {
  Reactor reactor = program_default_reactor_new("*", 2, 1);
  reactors_put(reactors, "*", reactor);
}

static void init_greater_than_reactor(Reactors *reactors) {
  Reactor reactor = program_default_reactor_new(">", 2, 1);
  reactors_put(reactors, ">", reactor);
}

static void init_even_reactor(Reactors *reactors) {
  Reactor reactor = program_default_reactor_new("even?", 1, 1);
  reactors_put(reactors, "even?", reactor);
}

static void init_if_reactors(Reactors *reactors) {
  Reactor reactor = program_default_reactor_new("if*", 3, 1);
  reactors_put(reactors, "if?", reactor);
  reactors_put(reactors, "if*", reactor);
}

static void init_foo_reactor(Reactors *reactors) {
  Reactor reactor = program_default_reactor_new("foo", 2, 1);
  reactors_put(reactors, "foo", reactor);
}
static void init_bar_reactor(Reactors *reactors) {
  Reactor reactor = program_default_reactor_new("bar", 2, 1);
  reactors_put(reactors, "bar", reactor);
}

Program program_new(Reactors reactors) {
  // + Reactor
  init_plus_reactor(&reactors);
  // - Reactor
  init_minus_reactor(&reactors);
  // * Reactor
  init_mul_reactor(&reactors);
  // > Reactor
  init_greater_than_reactor(&reactors);
  // even? Reactor
  init_even_reactor(&reactors);
  // if? and if* Reactor
  init_if_reactors(&reactors);
  // foo Reactor
  init_foo_reactor(&reactors);
  // bar Reactor
  init_bar_reactor(&reactors);
  return (Program){.reactors = reactors};
}

Reactors program_get_reactors(Program *program) { return program->reactors; }
