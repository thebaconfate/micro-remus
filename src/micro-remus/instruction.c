#include "instruction.h"
#include "abstractions.h"
#include "branch.h"
#include "location.h"
#include "operand.h"
#include "option.h"
#include "remus.h"
#include "types.h"
#include "value.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define STREQ(a, b) (strcmp((a), (b)) == 0)

static void command_handle_alloc_mono(Remus *remus,
                                      DeploymentId current_deployment_id) {
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void command_handle_def_rho(Instruction *command, Remus *remus,
                                   DeploymentId current_deployment_id) {
  DefRho def_rho = command->as.def_rho;
  Value reactor = {.type = VAL_REACTOR, .as.reactor = def_rho.name};
  remus_write(remus, current_deployment_id, reactor);
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void command_handle_alloc_rho(Remus *remus,
                                     DeploymentId current_deployment_id) {
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void command_handle_trampoline(Instruction *command, Remus *remus,
                                      DeploymentId current_deployment_id) {
  Trampoline trampoline = command->as.trampoline;
  remus_write(remus, current_deployment_id, trampoline.value);
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void command_handle_supply(Instruction *command, Remus *remus,
                                  DeploymentId current_deployment_id) {
  Supply supply = command->as.supply;
  ValueOption location_option, operand_option;
  DeploymentId deployment_id;
  location_option =
      location_fetch(&supply.location, current_deployment_id, remus);
  switch (location_option.option_tag) {
  case SOME:
    deployment_id = location_option.value->as.number;
    operand_option =
        operand_fetch(&supply.operand, current_deployment_id, remus);
    switch (operand_option.option_tag) {
    case SOME:
      remus_set_input(remus, deployment_id, supply.number,
                      *operand_option.value);
      remus_increment_pc(remus, current_deployment_id);
      remus_react(remus, current_deployment_id);
      break;
    case NONE:
      fprintf(stderr, "Error: Expected a value from operand\n");
      exit(EXIT_FAILURE);
      break;
    }
    break;
  case NONE:
    fprintf(stderr, "Error: Expected a location containing a deployment id\n");
    exit(EXIT_FAILURE);
    break;
  default:
    fprintf(stderr, "Error: Unexpected error during handling CMD_SUPPLY\n");
    exit(EXIT_FAILURE);
  }
}

static void command_handle_update(Instruction *command, Remus *remus,
                                  DeploymentId current_deployment_id) {
  Update update = command->as.update;
  ValueOption operand_option;
  switch (update.location.type) {
  case LOC_D:
    operand_option =
        operand_fetch(&update.operand, current_deployment_id, remus);
    switch (operand_option.option_tag) {
    case SOME:
      remus_update_trampoline(remus, current_deployment_id,
                              update.location.index, *operand_option.value);
      remus_increment_pc(remus, current_deployment_id);
      remus_react(remus, current_deployment_id);
      break;
    case NONE:
    default:
      fprintf(stderr, "Error: Expected value from operand\n");
      exit(EXIT_FAILURE);
      break;
    }
    break;
  case LOC_I:
  case LOC_O:
  case LOC_R:
  default:
    fprintf(stderr, "Error: A trampoline variable should be stored in the "
                    "deployment memory\n");
    exit(EXIT_FAILURE);
    break;
  }
}

static void command_handle_scan(Instruction *command, Remus *remus,
                                DeploymentId current_deployment_id) {
  Scan scan = command->as.scan;
  DeploymentId deployment_id =
      remus_get_deployment_in_scope(remus, current_deployment_id, scan.number);
  ValueOption operand_option;
  operand_option = operand_fetch(&scan.operand, deployment_id, remus);
  switch (operand_option.option_tag) {
  case SOME:
    remus_write(remus, current_deployment_id, *operand_option.value);
    remus_increment_pc(remus, current_deployment_id);
    remus_react(remus, current_deployment_id);
    break;
  case NONE:
  default:
    fprintf(stderr,
            "Error: Expected the operand to contain a value, got NONE\n");
    exit(EXIT_FAILURE);
  }
}

static void command_handle_react(Instruction *command, Remus *remus,
                                 DeploymentId current_deployment_id) {
  React react = command->as.react;
  ValueOption locationOption;
  DeploymentId deployment_id;
  locationOption =
      location_fetch(&react.location, current_deployment_id, remus);
  switch (locationOption.option_tag) {
  case SOME:
    deployment_id = locationOption.value->as.number;
    remus_increment_pc(remus, current_deployment_id);
    remus_set_return_address(remus, deployment_id, current_deployment_id);
    remus_initialize_pc(remus, deployment_id);
    remus_react(remus, deployment_id);
  case NONE:
  default:
    fprintf(stderr, "Error: Expected a location containing a deployment id\n");
    exit(EXIT_FAILURE);
  }
}

static void command_handle_consume(Instruction *command, Remus *remus,
                                   DeploymentId current_deployment_id) {
  Consume consume = command->as.consume;
  ValueOption location_option, output_option;
  DeploymentId deployment_id;
  location_option =
      location_fetch(&consume.location, current_deployment_id, remus);
  switch (location_option.option_tag) {
  case SOME:
    deployment_id = location_option.value->as.number;
    output_option = remus_get_output(remus, deployment_id, consume.number);
    switch (output_option.option_tag) {
    case SOME:
      remus_write(remus, current_deployment_id, *output_option.value);
      remus_increment_pc(remus, current_deployment_id);
      remus_react(remus, current_deployment_id);
      break;
    case NONE:
    default:
      fprintf(stderr,
              "Error: Expected a value in the %dth output of the deployment\n",
              (int)deployment_id);
      exit(EXIT_FAILURE);
    }
  case NONE:
  default:
    fprintf(stderr, "Error: Expected a location containing a deployment id\n");
    exit(EXIT_FAILURE);
  }
}

static void command_handle_global(Instruction *command, Remus *remus,
                                  DeploymentId current_deployment_id) {
  Global global = command->as.global;
  ValueOption value_option = remus_get_signal(remus, global.name);
  switch (value_option.option_tag) {
  case SOME:
    remus_write(remus, current_deployment_id, *value_option.value);
    remus_increment_pc(remus, current_deployment_id);
    remus_react(remus, current_deployment_id);
    break;
  case NONE:
  default:
    fprintf(stderr, "Error: Expected a value for the global signal %s\n",
            value_option.value->as.reactor);
    exit(EXIT_FAILURE);
  }
}

static void command_handle_read(Instruction *command, Remus *remus,
                                DeploymentId current_deployment_id) {
  Read read = command->as.read;
  ValueOption location_option;
  Value value;
  switch (read.location.type) {
  case LOC_D:
    location_option =
        location_fetch(&read.location, current_deployment_id, remus);
    switch (location_option.option_tag) {
    case SOME:
      remus_write(remus, current_deployment_id, *location_option.value);
      remus_increment_pc(remus, current_deployment_id);
      remus_react(remus, current_deployment_id);
    case NONE:
    default:
      fprintf(stderr,
              "Error: expected trampoline variable to contain a value\n");
      exit(EXIT_FAILURE);
    }
  case LOC_I:
  case LOC_O:
  case LOC_R:
  default:
    fprintf(stderr, "Error: A trampoline variable should be stored in the "
                    "deployment memory\n");
    exit(EXIT_FAILURE);
  }
}

static void command_handle_sink(Instruction *command, Remus *remus,
                                DeploymentId current_deployment_id) {
  Sink sink = command->as.sink;
  ValueOption operand_option;
  operand_option = operand_fetch(&sink.operand, current_deployment_id, remus);
  switch (operand_option.option_tag) {
  case SOME:
    remus_set_output(remus, current_deployment_id, sink.number,
                     *operand_option.value);
    remus_increment_pc(remus, current_deployment_id);
    remus_react(remus, current_deployment_id);
    break;
  case NONE:
  default:
    fprintf(stderr, "Error: Expected value in the operand\n");
    exit(EXIT_FAILURE);
  }
}

static void command_handle_make_poly(Remus *remus,
                                     DeploymentId current_deployment_id) {
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
  return;
}

static void command_handle_alloc_poly(Instruction *command, Remus *remus,
                                      DeploymentId current_deployment_id) {
  AllocPoly alloc_poly = command->as.alloc_poly;
  ValueOption operand_option, location_option, value_option;
  Value reactor_name, branch;
  BranchEntry branchEntry;
  operand_option =
      operand_fetch(&alloc_poly.operand, current_deployment_id, remus);
  switch (operand_option.option_tag) {
  case SOME:
    reactor_name = *operand_option.value;
    location_option =
        location_fetch(&alloc_poly.location, current_deployment_id, remus);
    switch (location_option.option_tag) {
    case SOME:
      branch = *location_option.value;
      value_option = branch_find(branch.as.branch, reactor_name.as.reactor);
      switch (value_option.option_tag) {
      case SOME:
        remus_write(remus, current_deployment_id, *value_option.value);
        break;
      default:
        fprintf(stderr,
                "Error: The reactor should already have been deployed\n");
        exit(EXIT_FAILURE);
        break;
      }
      remus_increment_pc(remus, current_deployment_id);
      remus_react(remus, current_deployment_id);
      break;
    default:
      fprintf(stderr, "Error: Expected a branching point\n");
      exit(EXIT_FAILURE);
      break;
    }
  default:
    fprintf(stderr, "Error: Expected to fetch a rho from the operand\n");
    exit(EXIT_FAILURE);
    break;
  }
}

static void primitive_handle_plus(Remus *remus,
                                  DeploymentId current_deployment_id,
                                  Inputs *inputs) {
  Value sum = (Value){.type = VAL_NUMBER, .as.number = 0};
  ValueOption option;
  for (size_t i = 0; i < inputs->len; i++) {
    option = inputs->storage[i];
    if (option.option_tag == SOME) {
      sum = value_add(sum, *option.value);
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
  Value operand1, operand2, subtraction_result;
  switch (inputs->storage[0].option_tag) {
  case SOME:
    switch (inputs->storage[0].value->type) {
    case VAL_NUMBER:
      operand1 = *inputs->storage[0].value;
      switch (inputs->storage[1].option_tag) {
      case SOME:
        switch (inputs->storage[1].value->type) {
        case VAL_NUMBER:
          operand2 = *inputs->storage[1].value;
          subtraction_result =
              (Value){.type = VAL_NUMBER,
                      .as.number = operand1.as.number - operand2.as.number};
          remus_write(remus, current_deployment_id, subtraction_result);
          break;
        default:
          fprintf(stderr,
                  "Error: Expected a number as a second argument for - \n");
          exit(EXIT_FAILURE);
        }
        break;
      default:
        fprintf(stderr,
                "Error: Expected a second argument for - gotten: NONE \n");
        exit(EXIT_FAILURE);
      }
      break;
    default:
      fprintf(stderr, "Error: Expected a number as a first argument for - \n");
      exit(EXIT_FAILURE);
    }
    break;
  default:
    fprintf(stderr, "Error: Expected a first argument for - gotten: NONE\n");
    exit(EXIT_FAILURE);
  }
}

static void primitive_handle_mul(Remus *remus,
                                 DeploymentId current_deployment_id,
                                 Inputs *inputs) {
  Value product = (Value){.type = VAL_NUMBER, .as.number = 1};
  ValueOption option;
  for (size_t i = 0; i < inputs->len; i++) {
    option = inputs->storage[i];
    if (option.option_tag == SOME) {
      product = value_mul(product, *option.value);
    }
  }
  remus_write(remus, current_deployment_id, product);
}

static void command_handle_primitive(Instruction *command, Remus *remus,
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
    if (inputs->len != 2) {
      fprintf(stderr, "Error: wrong arity for >, gotten %d", (int)inputs->len);
      exit(EXIT_FAILURE);
    }
    // TODO: implement this;
  } else if (STREQ(primitive.name, "even?")) {
    // TODO: implement this;
  } else if (STREQ(primitive.name, "if*")) {
    // TODO: implement this;
  } else if (STREQ(primitive.name, "foo")) {
    printf("foo was found");
  } else if (STREQ(primitive.name, "bar")) {
    printf("bar was found");
  } else {
    fprintf(stderr,
            "Error: µ-Remus does not understand the primitive operation %s\n",
            primitive.name);
    exit(EXIT_FAILURE);
  }
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

void command_execute(Instruction *command, DeploymentId current_deployment_id,
                     Remus *remus) {
  ValueOption option;
  switch (command->tag) {
  case CMD_ALLOC_MONO:
    command_handle_alloc_mono(remus, current_deployment_id);
    break;
  case CMD_DEF_RHO:
    command_handle_def_rho(command, remus, current_deployment_id);
    break;
  case CMD_ALLOC_RHO:
    command_handle_alloc_rho(remus, current_deployment_id);
    break;
  case CMD_TRAMPOLINE:
    command_handle_trampoline(command, remus, current_deployment_id);
    break;
  case CMD_SUPPLY:
    command_handle_supply(command, remus, current_deployment_id);
    break;
  case CMD_UPDATE:
    command_handle_update(command, remus, current_deployment_id);
    break;
  case CMD_SCAN:
    command_handle_scan(command, remus, current_deployment_id);
    break;
  case CMD_REACT:
    command_handle_react(command, remus, current_deployment_id);
    break;
  case CMD_CONSUME:
    command_handle_consume(command, remus, current_deployment_id);
    break;
  case CMD_GLOBAL:
    command_handle_global(command, remus, current_deployment_id);
    break;
  case CMD_READ:
    command_handle_read(command, remus, current_deployment_id);
    break;
  case CMD_SINK:
    command_handle_sink(command, remus, current_deployment_id);
    break;
  case CMD_MAKE_POLY:
    command_handle_make_poly(remus, current_deployment_id);
    break;
  case CMD_ALLOC_POLY:
    command_handle_alloc_poly(command, remus, current_deployment_id);
    break;
  case CMD_PRIMITIVE:
    command_handle_primitive(command, remus, current_deployment_id);
    break;
  default:
    fprintf(stderr, "Error: Invalid command type tag\n");
    exit(EXIT_FAILURE);
  }
  return;
}
