#include "command.h"
#include "location.h"
#include "operand.h"
#include "option.h"
#include "remus.h"
#include "types.h"
#include "value.h"
#include <stdio.h>
#include <stdlib.h>

static void command_handle_alloc_mono(Remus *remus,
                                      DeploymentId current_deployment_id) {
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void command_handle_def_rho(Command *command, Remus *remus,
                                   DeploymentId current_deployment_id) {
  DefRho def_rho = command->as.def_rho;
  remus_write(remus, current_deployment_id,
              (Value){.type = VAL_REACTOR, .as.reactor = def_rho.name});
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void command_handle_alloc_rho(Remus *remus,
                                     DeploymentId current_deployment_id) {
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void command_handle_trampoline(Command *command, Remus *remus,
                                      DeploymentId current_deployment_id) {
  Trampoline trampoline = command->as.trampoline;
  remus_write(remus, current_deployment_id, trampoline.value);
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void command_handle_supply(Command *command, Remus *remus,
                                  DeploymentId current_deployment_id) {
  Supply supply = command->as.supply;
  ValueOption locationOption, operandOption;
  locationOption =
      location_fetch(&supply.location, current_deployment_id, remus);
  switch (locationOption.option_tag) {
  case SOME:
    operandOption =
        operand_fetch(&supply.operand, current_deployment_id, remus);
    switch (operandOption.option_tag) {
    case SOME:
      remus_set_input(remus, operandOption.value->as.number, supply.number,
                      *operandOption.value);
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

static void command_handle_update(Command *command, Remus *remus,
                                  DeploymentId current_deployment_id) {
  Update update = command->as.update;
  ValueOption operand_option;
  Operand operand;
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
      // TODO: Continue from here
      break;
    default:
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
  }
};

void command_execute(Command *command, DeploymentId current_deployment_id,
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
    break;
  case CMD_REACT:
    break;
  case CMD_CONSUME:
    break;
  case CMD_GLOBAL:
    break;
  case CMD_READ:
    break;
  case CMD_SINK:
    break;
  case CMD_MAKE_POLY:
    break;
  case CMD_ALLOC_POLY:
    break;
  case CMD_PRIMITIVE:
    break;
  default:
    fprintf(stderr, "Error: Invalid command type tag\n");
    exit(EXIT_FAILURE);
  }
  return;
}
