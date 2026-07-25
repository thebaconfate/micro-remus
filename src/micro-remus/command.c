#include "command.h"
#include "location.h"
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
  remus_write(
      remus, current_deployment_id,
      (Value){.type = VAL_REACTOR, .as.reactor = command->as.def_rho.name});
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void command_handle_alloc_rho(Command *command, Remus *remus,
                                     DeploymentId current_deployment_id) {
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void command_handle_trampoline(Command *command, Remus *remus,
                                      DeploymentId current_deployment_id) {
  remus_write(remus, current_deployment_id, command->as.trampoline.value);
  remus_increment_pc(remus, current_deployment_id);
  remus_react(remus, current_deployment_id);
}

static void command_handle_supply(Command *command, Remus *remus,
                                  DeploymentId current_deployment_id) {
  // TODO: Continue from here
  ValueOption option = location_fetch(&command->as.supply.location,
                                      current_deployment_id, remus);
  switch (option.option_tag) {
  case SOME:
    break;
  case NONE:
    break;
  default:
    exit(EXIT_FAILURE);
  }
}

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
    command_handle_alloc_rho(command, remus, current_deployment_id);
    break;
  case CMD_TRAMPOLINE:
    command_handle_trampoline(command, remus, current_deployment_id);
    break;
  case CMD_SUPPLY:
    command_handle_supply(command, remus, current_deployment_id);
    break;
  case CMD_UPDATE:
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
