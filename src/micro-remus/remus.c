
#include "remus.h"
#include "abstractions.h"
#include "deployment.h"
#include "instruction.h"
#include "reactor.h"
#include "reactors.h"
#include "signal.h"
#include "types.h"
#include "value.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static Number remus_increment_time(Remus *remus);
static void remus_actuate(Remus *remus, Outputs outputs);
static bool remus_is_deployed(Remus *remus, DeploymentId deployment_id);
static DeploymentIdOption remus_get_return_address(Remus *remus,
                                                   DeploymentId deployment_id);
static bool remus_get_dirty_bit(Remus *remus, DeploymentId deployment_id);

Remus remus_new(Program program) {
  DeploymentId main_deployment_id = 1;
  Deployments deployments = deployments_new();
  SignalEnvironment global_signals = signal_new();
  Reactors reactors = program.reactors;
  signal_put(&global_signals, "time",
             (Value){.type = VAL_NUMBER, .as.number = 0});

  return (Remus){.main_deployment_id = main_deployment_id,
                 .reactors = reactors,
                 .deployments = deployments,
                 .global_signals = global_signals};
}

void remus_start(Remus *remus, void (*sleep)(uint32_t ms)) {
  remus_increment_time(remus);
  remus_initialize_pc(remus, remus->main_deployment_id);
  remus_set_dirty_bit(remus, remus->main_deployment_id, true);
  remus_react(remus, remus->main_deployment_id);
  Outputs outputs = remus_get_outputs(remus, remus->main_deployment_id);
  remus_actuate(remus, outputs);
  sleep(1000);
}

static Number remus_increment_time(Remus *remus) {
  ValueOption time_option = remus_get_signal(remus, "time");
  if (time_option == NULL) {
    fprintf(stderr, "Error: expected time signal, gotten NULL\n");
    exit(EXIT_FAILURE);
  }
  if (time_option->type != VAL_NUMBER) {
    fprintf(stderr, "Error: expected a nuumber in the time signal, gotten %s",
            value_type_to_string(time_option->type));
    exit(EXIT_FAILURE);
  }
  return time_option->as.number++;
}
static void remus_actuate(Remus *remus, Outputs outputs) {
  size_t val_len = 0;

  for (size_t i = 0; i < outputs.len; i++) {
    if (outputs.storage[i] != NULL)
      val_len++;
  }

  Value *val = malloc(sizeof(Value) * val_len);
  val_len = 0;
  for (size_t i = 0; i < outputs.len; i++) {
    if (outputs.storage[i] != NULL) {
      val[val_len] = *outputs.storage[i];
      val_len++;
    }
  }

  printf("[");
  for (size_t i = 0; i < val_len - 1; i++) {
    print_value(&val[i]);
  }
  printf("]\n");
}

/*----------- reactors -------------*/
Reactor *remus_get_reactor(Remus *remus, Name reactor_name) {
  return reactors_get(remus->reactors, reactor_name);
}
void remus_deploy(Remus *remus, Name reactor_name, DeploymentId deployment_id,
                  Scope scope) {
  Reactor *reactor = reactors_get(remus->reactors, reactor_name);
  if (reactor == NULL) {
    fprintf(stderr, "Error: Could not find reactor %s\n", reactor_name);
    exit(EXIT_FAILURE);
  }
  Deployment deployment =
      deployment_new(reactor_name, *reactor, deployment_id, scope);
  remus_insert_deployment(remus, deployment_id, deployment);
}

/*----------- signals -------------*/

ValueOption remus_get_signal(Remus *remus, Name signal_name) {
  ValueOption signal_option = signal_get(remus->global_signals, signal_name);
  if (signal_option != NULL) {
    return signal_option;
  } else {
    // TODO: read signal from the hardware
    return NULL;
  }
}

/*----------- deployments -------------*/

Deployment *remus_get_deployment(Remus *remus, DeploymentId deployment_id) {
  Deployment *deployment = deployments_get(remus->deployments, deployment_id);
  if (deployment == NULL) {
    fprintf(stderr, "Error: could not find deployment %zu\n", deployment_id);
    exit(EXIT_FAILURE);
  }
  return deployment;
}
void remus_insert_deployment(Remus *remus, DeploymentId deployment_id,
                             Deployment deployment) {
  deployments_put(&remus->deployments, deployment_id, deployment);
}
static bool remus_is_deployed(Remus *remus, DeploymentId deployment_id) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  return deployment->is_deployed;
}
void remus_set_deployed(Remus *remus, DeploymentId deployment_id) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  deployment_set_deployed(deployment);
}
void remus_initialize_pc(Remus *remus, DeploymentId deployment_id) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  deployment_initialize_pc(deployment);
}
void remus_increment_pc(Remus *remus, DeploymentId deployment_id) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  deployment_increment_pc(deployment);
}
static DeploymentIdOption remus_get_return_address(Remus *remus,
                                                   DeploymentId deployment_id) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  return deployment->return_address;
}
void remus_set_return_address(Remus *remus, DeploymentId deployment_id,
                              DeploymentId return_address) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  deployment_set_return_address(deployment, return_address);
}
static bool remus_get_dirty_bit(Remus *remus, DeploymentId deployment_id) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  return deployment->dirty_bit;
}

void remus_set_dirty_bit(Remus *remus, DeploymentId deployment_id, bool dirty) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  deployment_set_dirty_bit(deployment, dirty);
}

bool remus_is_dirty(Remus *remus, DeploymentId deployment_id) {
  Scope scope = remus_get_scope(remus, deployment_id);
  bool dirty_bit = remus_get_dirty_bit(remus, deployment_id);
  for (size_t i = 0; i < scope.len; i++) {
    if (dirty_bit)
      return dirty_bit;
    DeploymentId child_id = scope.storage[i];
    dirty_bit |= remus_is_dirty(remus, child_id);
  }
  return dirty_bit;
}

Scope remus_get_scope(Remus *remus, DeploymentId deployment_id) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  return deployment_get_scope(deployment);
}

DeploymentId remus_get_deployment_in_scope(Remus *remus,
                                           DeploymentId deployment_id,
                                           Number depth) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  return deployment_get_deployment_in_scope(deployment, depth);
}

ValueOption remus_get_input(Remus *remus, DeploymentId deployment_id,
                            Number n) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  return deployment_get_input(deployment, n);
}
Inputs *remus_get_inputs(Remus *remus, DeploymentId deployment_id) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  return deployment_get_inputs(deployment);
}

void remus_set_input(Remus *remus, DeploymentId deployment_id, Number n,
                     Value value) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  deployment_set_input(deployment, n, value);
}

ValueOption remus_get_output(Remus *remus, DeploymentId deployment_id,
                             Number n) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  return deployment_get_output(deployment, n);
}

Outputs remus_get_outputs(Remus *remus, DeploymentId deployment_id) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  return deployment_get_outputs(deployment);
}

void remus_set_output(Remus *remus, DeploymentId deployment_id, Number n,
                      Value value) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  deployment_set_output(deployment, n, value);
}

Instruction remus_get_deployment_instruction(Remus *remus,
                                             DeploymentId deployment_id) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  Reactor *reactor = remus_get_reactor(remus, deployment->reactor_name);
  return reactor_get_deployment_instruction(reactor, deployment->pc);
}

ValueOption remus_read_d(Remus *remus, DeploymentId deployment_id, Number n) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  return deployment_read_d(deployment, n);
}

Instruction remus_get_reaction_instruction(Remus *remus,
                                           DeploymentId deployment_id) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  Reactor *reactor = remus_get_reactor(remus, deployment->reactor_name);
  return reactor_get_reaction_instruction(reactor, deployment->pc);
}

ValueOption remus_read_r(Remus *remus, DeploymentId deployment_id, Number n) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  return deployment_read_r(deployment, n);
}

void remus_write(Remus *remus, DeploymentId deployment_id, Value value) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  deployment_write(deployment, value);
}

void remus_write_at(Remus *remus, DeploymentId deployment_id, Number index,
                    Value value) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  deployment_write_at(deployment, index, value);
}

void remus_update_trampoline(Remus *remus, DeploymentId deployment_id,
                             Number location, Value value) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  deployment_update_trampoline(deployment, location, value);
}

Instruction remus_retrieve_next_instruction(Remus *remus,
                                            DeploymentId deployment_id) {
  if (remus_is_deployed(remus, deployment_id))
    return remus_get_reaction_instruction(remus, deployment_id);
  else
    return remus_get_deployment_instruction(remus, deployment_id);
}

bool remus_has_finished_phase(Remus *remus, DeploymentId deployment_id) {
  Deployment *deployment = remus_get_deployment(remus, deployment_id);
  return deployment_has_finished_phase(deployment);
}

void remus_finish_deployment_phase(Remus *remus, DeploymentId deployment_id) {
  remus_set_deployed(remus, deployment_id);
  remus_initialize_pc(remus, deployment_id);
  remus_react(remus, deployment_id);
}

void remus_finish_reaction_phase(Remus *remus, DeploymentId deployment_id) {
  remus_set_dirty_bit(remus, deployment_id, false);
  DeploymentIdOption deployment_id_option =
      remus_get_return_address(remus, deployment_id);
  if (deployment_id_option != NULL) {
    remus_react(remus, *deployment_id_option);
  }
}

void remus_finish_phase(Remus *remus, DeploymentId deployment_id) {
  if (remus_is_deployed(remus, deployment_id))
    remus_finish_reaction_phase(remus, deployment_id);
  else
    remus_finish_deployment_phase(remus, deployment_id);
}

void remus_react(Remus *remus, DeploymentId deployment_id) {
  if (remus_has_finished_phase(remus, deployment_id) ||
      remus_is_dirty(remus, deployment_id))
    remus_finish_phase(remus, deployment_id);
  else {
    Instruction instruction =
        remus_retrieve_next_instruction(remus, deployment_id);
    instruction_execute(&instruction, deployment_id, remus);
  }
}
