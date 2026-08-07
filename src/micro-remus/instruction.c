#include "instruction.h"
#include "abstractions.h"
#include "branch.h"
#include "location.h"
#include "operand.h"
#include "remus.h"
#include "types.h"
#include "value.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define STREQ(a, b) (strcmp((a), (b)) == 0)

static void intrsuction_handle_alloc_mono(Remus *remus,
                                          DeploymentId current_deployment_id) {
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void instruction_handle_def_rho(Instruction *command, Remus *remus,
                                       DeploymentId current_deployment_id) {
  DefRho def_rho = command->as.def_rho;
  Value reactor = {.type = VAL_REACTOR, .as.reactor = def_rho.name};
  remus_write(remus, current_deployment_id, reactor);
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void instruction_handle_alloc_rho(Remus *remus,
                                         DeploymentId current_deployment_id) {
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void instruction_handle_trampoline(Instruction *command, Remus *remus,
                                          DeploymentId current_deployment_id) {
  Trampoline trampoline = command->as.trampoline;
  remus_write(remus, current_deployment_id, trampoline.value);
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void instruction_handle_supply(Instruction *command, Remus *remus,
                                      DeploymentId current_deployment_id) {
  Supply supply = command->as.supply;
  ValueOption location, operand;
  DeploymentId deployment_id;
  location = location_fetch(&supply.location, current_deployment_id, remus);
  if (location == NULL) {
    fprintf(stderr, "Error: Expected a location containing a deployment id\n");
    exit(EXIT_FAILURE);
  }
  deployment_id = location->as.number;
  operand = operand_fetch(&supply.operand, current_deployment_id, remus);
  if (operand == NULL) {
    fprintf(stderr, "Error: Expected a value from operand\n");
    exit(EXIT_FAILURE);
  }
  remus_set_input(remus, deployment_id, supply.number, *operand);
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void instruction_handle_update(Instruction *command, Remus *remus,
                                      DeploymentId current_deployment_id) {
  Update update = command->as.update;
  ValueOption operand;
  switch (update.location.type) {
  case LOC_D:
    operand = operand_fetch(&update.operand, current_deployment_id, remus);
    if (operand == NULL) {
      fprintf(stderr, "Error: Expected value from operand\n");
      exit(EXIT_FAILURE);
    }
    remus_update_trampoline(remus, current_deployment_id, update.location.index,
                            *operand);
    remus_increment_pc(remus, current_deployment_id);
    remus_react(remus, current_deployment_id);
    break;
  default:
    fprintf(stderr, "Error: A trampoline variable should be stored in the "
                    "deployment memory\n");
    exit(EXIT_FAILURE);
    break;
  }
}

static void instruction_handle_scan(Instruction *command, Remus *remus,
                                    DeploymentId current_deployment_id) {
  Scan scan = command->as.scan;
  DeploymentId deployment_id =
      remus_get_deployment_in_scope(remus, current_deployment_id, scan.number);
  ValueOption operand;
  operand = operand_fetch(&scan.operand, deployment_id, remus);
  if (operand == NULL) {
    fprintf(stderr,
            "Error: Expected the operand to contain a value, got NONE\n");
    exit(EXIT_FAILURE);
  }
  remus_write(remus, current_deployment_id, *operand);
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void instruction_handle_react(Instruction *command, Remus *remus,
                                     DeploymentId current_deployment_id) {
  React react = command->as.react;
  ValueOption location;
  DeploymentId deployment_id;
  location = location_fetch(&react.location, current_deployment_id, remus);
  if (location == NULL) {
    fprintf(stderr, "Error: Expected a location containing a deployment id\n");
    exit(EXIT_FAILURE);
  }
  deployment_id = location->as.number;
  remus_increment_pc(remus, current_deployment_id);
  remus_set_return_address(remus, deployment_id, current_deployment_id);
  remus_initialize_pc(remus, deployment_id);
  remus_react(remus, deployment_id);
}

static void instruction_handle_consume(Instruction *command, Remus *remus,
                                       DeploymentId current_deployment_id) {
  Consume consume = command->as.consume;
  ValueOption location, output;
  DeploymentId deployment_id;
  location = location_fetch(&consume.location, current_deployment_id, remus);
  if (location == NULL) {
    fprintf(stderr, "Error: Expected a location containing a deployment id\n");
    exit(EXIT_FAILURE);
  }
  deployment_id = location->as.number;
  output = remus_get_output(remus, deployment_id, consume.number);
  if (output == NULL) {
    fprintf(stderr,
            "Error: Expected a value in the %dth output of the deployment\n",
            (int)deployment_id);
    exit(EXIT_FAILURE);
  }
  remus_write(remus, current_deployment_id, *output);
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void instruction_handle_global(Instruction *command, Remus *remus,
                                      DeploymentId current_deployment_id) {
  Global global = command->as.global;
  ValueOption signal = remus_get_signal(remus, global.name);
  if (signal == NULL) {
    fprintf(stderr, "Error: Expected a value for the global signal %s\n",
            global.name);
    exit(EXIT_FAILURE);
  }
  remus_write(remus, current_deployment_id, *signal);
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void instruction_handle_read(Instruction *command, Remus *remus,
                                    DeploymentId current_deployment_id) {
  Read read = command->as.read;
  ValueOption location;
  Value value;
  switch (read.location.type) {
  case LOC_D:
    location = location_fetch(&read.location, current_deployment_id, remus);
    if (location == NULL) {
      fprintf(stderr,
              "Error: expected trampoline variable to contain a value\n");
      exit(EXIT_FAILURE);
    }
    remus_write(remus, current_deployment_id, *location);
    remus_increment_pc(remus, current_deployment_id);
    remus_react(remus, current_deployment_id);
    break;
  default:
    fprintf(stderr, "Error: A trampoline variable should be stored in the "
                    "deployment memory\n");
    exit(EXIT_FAILURE);
  }
}

static void instruction_handle_sink(Instruction *command, Remus *remus,
                                    DeploymentId current_deployment_id) {
  Sink sink = command->as.sink;
  ValueOption operand;
  operand = operand_fetch(&sink.operand, current_deployment_id, remus);
  if (operand == NULL) {
    fprintf(stderr, "Error: Expected value in the operand\n");
    exit(EXIT_FAILURE);
  }
  remus_set_output(remus, current_deployment_id, sink.number, *operand);
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void instruction_handle_make_poly(Remus *remus,
                                         DeploymentId current_deployment_id) {
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
  return;
}

static void instruction_handle_alloc_poly(Instruction *command, Remus *remus,
                                          DeploymentId current_deployment_id) {
  AllocPoly alloc_poly = command->as.alloc_poly;
  ValueOption operand, location, value;
  Value reactor_name, branch;
  BranchEntry branchEntry;
  operand = operand_fetch(&alloc_poly.operand, current_deployment_id, remus);
  if (operand == NULL) {
    fprintf(stderr, "Error: Expected to fetch a rho from the operand\n");
    exit(EXIT_FAILURE);
  }
  reactor_name = *operand;
  location = location_fetch(&alloc_poly.location, current_deployment_id, remus);
  if (location == NULL) {
    fprintf(stderr, "Error: Expected a branching point\n");
    exit(EXIT_FAILURE);
  }
  branch = *location;
  value = branch_find(branch.as.branch, reactor_name.as.reactor);
  if (value == NULL) {
    fprintf(stderr, "Error: The reactor should already have been deployed\n");
    exit(EXIT_FAILURE);
  }
  remus_write(remus, current_deployment_id, *value);
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void primitive_handle_plus(Remus *remus,
                                  DeploymentId current_deployment_id,
                                  Inputs *inputs) {
  Value sum = (Value){.type = VAL_NUMBER, .as.number = 0};
  ValueOption option;
  for (size_t i = 0; i < inputs->len; i++) {
    option = inputs->storage[i];
    if (option != NULL) {
      sum = value_add(sum, *option);
    }
  }
  remus_write(remus, current_deployment_id, sum);
}

static void primitive_handle_minus(Remus *remus,
                                   DeploymentId current_deployment_id,
                                   Inputs *inputs) {
  if (inputs->len < 2) {
    fprintf(stderr, "Error: operation - requires at least 2 inputs\n");
    exit(EXIT_FAILURE);
  }
  ValueOption operand1, operand2;
  Value subtraction_result;
  operand1 = inputs->storage[0];
  if (operand1 == NULL) {
    fprintf(stderr, "Error: Expected a first argument for - gotten: NONE\n");
    exit(EXIT_FAILURE);
  }
  if (operand1->type != VAL_NUMBER) {
    fprintf(stderr, "Error: Expected a number as a first argument for - \n");
    exit(EXIT_FAILURE);
  }
  operand2 = inputs->storage[1];
  if (operand2 == NULL) {
    fprintf(stderr, "Error: Expected a second argument for - gotten: NONE \n");
    exit(EXIT_FAILURE);
  }
  if (operand2->type != VAL_NUMBER) {
    fprintf(stderr, "Error: Expected a number as a second argument for - \n");
    exit(EXIT_FAILURE);
  }
  subtraction_result =
      (Value){.type = VAL_NUMBER,
              .as.number = operand1->as.number - operand2->as.number};
  remus_write(remus, current_deployment_id, subtraction_result);
}

static void primitive_handle_mul(Remus *remus,
                                 DeploymentId current_deployment_id,
                                 Inputs *inputs) {
  Value product = (Value){.type = VAL_NUMBER, .as.number = 1};
  ValueOption option;
  for (size_t i = 0; i < inputs->len; i++) {
    option = inputs->storage[i];
    if (option != NULL) {
      product = value_mul(product, *option);
    }
  }
  remus_write(remus, current_deployment_id, product);
}

static void primitive_handle_greater_than(Remus *remus,
                                          DeploymentId current_deployment_id,
                                          Inputs *inputs) {
  size_t input_size = inputs->len;
  ValueOption *args = inputs->storage;
  ValueOption x_option, y_option;
  Value result;
  Number x, y;
  if (input_size != 2) {
    fprintf(stderr, "Error: Wrong arity. > requires 2 arguments");
    exit(EXIT_FAILURE);
  }
  x_option = args[0];
  if (x_option == NULL) {
    fprintf(stderr, "Error: Expected a value as first argument of >. Got NONE");
    exit(EXIT_FAILURE);
  }
  if (x_option->type != VAL_NUMBER) {
    const char *type = value_type_to_string(x_option->type);
    fprintf(stderr, "Error: > Expected NUMBER as first argument got: %s", type);
    exit(EXIT_FAILURE);
  }
  x = x_option->as.number;
  y_option = args[1];
  if (y_option == NULL) {
    fprintf(stderr,
            "Error: Expected a value as second argument of >. Got NONE");
    exit(EXIT_FAILURE);
  }
  if (y_option->type != VAL_NUMBER) {
    const char *type = value_type_to_string(y_option->type);
    fprintf(stderr, "Error: > Expected NUMBERR as second argument got: %s",
            type);
    exit(EXIT_FAILURE);
  }
  y = y_option->as.number;
  result = (Value){.type = VAL_BOOLEAN, .as.boolean = x > y};
  remus_write(remus, current_deployment_id, result);
}

static void primitive_handle_even(Remus *remus,
                                  DeploymentId current_deployment_id,
                                  Inputs *inputs) {
  size_t input_len = inputs->len;
  ValueOption option;
  Value value, write_value;
  Number number;
  bool result = true;
  for (size_t i = 0; i < inputs->len; i++) {
    option = inputs->storage[i];
    if (option == NULL) {
      fprintf(stderr,
              "Error: Expected a number to test for even, gotten NONE\n");
      exit(EXIT_FAILURE);
    }
    value = *option;
    if (value.type != VAL_NUMBER) {
      fprintf(stderr, "Error: Expected a number to test for even, gotten %s\n",
              value_type_to_string(value.type));
      exit(EXIT_FAILURE);
    }
    result &= (value.as.number % 2 == 0);
  }
  write_value = (Value){.type = VAL_BOOLEAN, .as.boolean = result};
  remus_write(remus, current_deployment_id, write_value);
}

static void primitive_handle_if_star(Remus *remus,
                                     DeploymentId current_deployment_id,
                                     Inputs *inputs) {
  size_t input_len = inputs->len;
  ValueOption option, consequent_option, alternative_option;
  Value condition, consequent, alternative;
  if (input_len < 1) {
    fprintf(stderr, "Error: Expected boolean, no condition in inputs\n");
    exit(EXIT_FAILURE);
  }
  option = inputs->storage[0];
  if (option == NULL) {
    fprintf(stderr, "Error: Expected boolean, gotten NONE \n");
    exit(EXIT_FAILURE);
  }
  condition = *option;
  if (condition.type != VAL_BOOLEAN) {
    fprintf(stderr, "Error: Expected boolean, gotten %s \n",
            value_type_to_string(condition.type));
    exit(EXIT_FAILURE);
  }
  if (condition.as.boolean) {
    if (input_len < 2) {
      fprintf(stderr, "Error: Expected consequent, no consequent in inputs \n");
      exit(EXIT_FAILURE);
    }
    consequent_option = inputs->storage[1];
    if (consequent_option == NULL) {
      fprintf(stderr, "Error: Expected consequent, gotten NONE\n");
      exit(EXIT_FAILURE);
    }
    consequent = *consequent_option;
    remus_write(remus, current_deployment_id, consequent);
  } else {
    if (input_len < 3) {
      fprintf(stderr,
              "Error: Expected alternative, no alternative in inputs \n");
      exit(EXIT_FAILURE);
    }
    alternative_option = inputs->storage[2];
    if (alternative_option == NULL) {
      fprintf(stderr, "Error: Expected alternative, gotten NONE\n");
      exit(EXIT_FAILURE);
    }
    alternative = *alternative_option;
    remus_write(remus, current_deployment_id, alternative);
  }
}

static void instruction_handle_primitive(Instruction *command, Remus *remus,
                                         DeploymentId current_deployment_id) {
  Primitive primitive = command->as.primitive;
  Inputs *inputs = remus_get_inputs(remus, current_deployment_id);
  if (STREQ(primitive.name, "+")) {
    primitive_handle_plus(remus, current_deployment_id, inputs);
  } else if (STREQ(primitive.name, "-")) {
    primitive_handle_minus(remus, current_deployment_id, inputs);
  } else if (STREQ(primitive.name, "*")) {
    primitive_handle_mul(remus, current_deployment_id, inputs);
  } else if (STREQ(primitive.name, ">")) {
    primitive_handle_greater_than(remus, current_deployment_id, inputs);
  } else if (STREQ(primitive.name, "even?")) {
    primitive_handle_even(remus, current_deployment_id, inputs);
  } else if (STREQ(primitive.name, "if*")) {
    primitive_handle_if_star(remus, current_deployment_id, inputs);
  } else if (STREQ(primitive.name, "foo")) {
    printf("foo was found\n");
  } else if (STREQ(primitive.name, "bar")) {
    printf("bar was found\n");
  } else {
    fprintf(stderr,
            "Error: µ-Remus does not understand the primitive operation %s\n",
            primitive.name);
    exit(EXIT_FAILURE);
  }
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

void instruction_execute(Instruction *command,
                         DeploymentId current_deployment_id, Remus *remus) {
  ValueOption option;
  switch (command->tag) {
  case CMD_ALLOC_MONO:
    intrsuction_handle_alloc_mono(remus, current_deployment_id);
    break;
  case CMD_DEF_RHO:
    instruction_handle_def_rho(command, remus, current_deployment_id);
    break;
  case CMD_ALLOC_RHO:
    instruction_handle_alloc_rho(remus, current_deployment_id);
    break;
  case CMD_TRAMPOLINE:
    instruction_handle_trampoline(command, remus, current_deployment_id);
    break;
  case CMD_SUPPLY:
    instruction_handle_supply(command, remus, current_deployment_id);
    break;
  case CMD_UPDATE:
    instruction_handle_update(command, remus, current_deployment_id);
    break;
  case CMD_SCAN:
    instruction_handle_scan(command, remus, current_deployment_id);
    break;
  case CMD_REACT:
    instruction_handle_react(command, remus, current_deployment_id);
    break;
  case CMD_CONSUME:
    instruction_handle_consume(command, remus, current_deployment_id);
    break;
  case CMD_GLOBAL:
    instruction_handle_global(command, remus, current_deployment_id);
    break;
  case CMD_READ:
    instruction_handle_read(command, remus, current_deployment_id);
    break;
  case CMD_SINK:
    instruction_handle_sink(command, remus, current_deployment_id);
    break;
  case CMD_MAKE_POLY:
    instruction_handle_make_poly(remus, current_deployment_id);
    break;
  case CMD_ALLOC_POLY:
    instruction_handle_alloc_poly(command, remus, current_deployment_id);
    break;
  case CMD_PRIMITIVE:
    instruction_handle_primitive(command, remus, current_deployment_id);
    break;
  default:
    fprintf(stderr, "Error: Invalid command type tag\n");
    exit(EXIT_FAILURE);
  }
  return;
}

Instructions instructions_new() {
  return (Instructions){.storage = NULL, .capacity = 0, .len = 0};
}

void instructions_add(Instructions *instructions, Instruction instruction) {
  if (instructions->len + 1 > instructions->capacity) {
    size_t new_capacity =
        instructions->capacity == 0 ? 4 : instructions->capacity * 2;
    Instruction *new_storage =
        realloc(instructions->storage, sizeof(Instruction) * new_capacity);
    instructions->capacity = new_capacity;
    instructions->storage = new_storage;
  }
  instructions->storage[instructions->len++] = instruction;
}
