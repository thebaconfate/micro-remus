
#include "remus.h"
#include "abstractions.h"
#include "instruction.h"
#include "types.h"
#include <stdbool.h>

// TODO: Actually implement these functions
Remus remus_new(Program program) {
  DeploymentId main_deployment_id = 1;
  // Deployments deployments = deployments_new();
  return (Remus){};
}
void remus_start(Remus *remus) {}
Number remus_increment_time(Remus *remus) { return (Number){}; }
void remus_actuate(Remus *remus, Outputs outputs) {
} // Check signature of outputs

/*----------- reactors -------------*/
Reactor *remus_get_reactor(Remus *remus, Name reactor_name) { return NULL; }
void remus_deploy(Remus *remus, Name reactor_name, DeploymentId deployment_id,
                  Scope Scope) {}

/*----------- signals -------------*/

ValueOption remus_get_signal(Remus *remus, Name signal_name) {
  return (ValueOption){};
}

/*----------- deployments -------------*/

Deployment remus_get_deployment(Remus *remus, DeploymentId deployment_id) {
  return (Deployment){};
}
void remus_insert_deployment(Remus *remus, DeploymentId deployment_id,
                             Deployment deployment) {}
bool remus_is_deployed(Remus *remus, DeploymentId deployment_id) {
  return true;
}
void remus_set_deployed(Remus *remus, DeploymentId deployment_id) {}
void remus_initialize_pc(Remus *remus, DeploymentId deployment_id) {}
void remus_increment_pc(Remus *remus, DeploymentId deployment_id) {}
DeploymentIdOption remus_get_return_address(Remus *remus,
                                            DeploymentId deployment_id) {
  return (DeploymentIdOption){};
}
void remus_set_return_address(Remus *remus, DeploymentId deployment_id,
                              DeploymentId return_address) {}
bool remus_get_dirty_bit(Remus *remus, DeploymentId deployment_id) {
  return true;
}

void remus_set_dirty_bit(Remus *remus, DeploymentId deployment_id, bool dirty) {
}

bool remus_is_dirty(Remus *remus, DeploymentId deployment_id) { return true; }

Scope remus_get_scope(Remus *remus, DeploymentId deployment_id) {
  return (Scope){};
}

DeploymentId remus_get_deployment_in_scope(Remus *remus,
                                           DeploymentId deployment_id,
                                           Number depth) {
  return (DeploymentId){};
}

ValueOption remus_get_input(Remus *remus, DeploymentId deployment_id,
                            Number n) {
  return (ValueOption){};
}
Inputs *remus_get_inputs(Remus *remus, DeploymentId deployment_id) {
  return NULL;
}

void remus_set_input(Remus *remus, DeploymentId deployment_id, Number n,
                     Value value) {}

ValueOption remus_get_output(Remus *remus, DeploymentId deployment_id,
                             Number n) {
  return (ValueOption){};
}

Outputs remus_get_outputs(Remus *remus, DeploymentId deployment_id) {
  return (Outputs){};
}

void remus_set_output(Remus *remus, DeploymentId deployment_id, Number n,
                      Value value) {}

Instruction remus_get_deployment_command(Remus *remus,
                                         DeploymentId deployment_id) {
  return (Instruction){};
}

ValueOption remus_read_d(Remus *remus, DeploymentId deployment_id, Number n) {
  return (ValueOption){};
}

Instruction remus_get_reaction_command(Remus *remus,
                                       DeploymentId deployment_id) {
  return (Instruction){};
}

ValueOption remus_read_r(Remus *remus, DeploymentId deployment_id, Number n) {
  return (ValueOption){};
}

void remus_write(Remus *remus, DeploymentId deployment_id, Value value) {}

void remus_write_at(Remus *remus, DeploymentId deployment_id, Number index,
                    Value value) {}

void remus_update_trampoline(Remus *remus, DeploymentId deployment_id,
                             Number location, Value value) {}

Instruction remus_retrieve_next_command(Remus *remus,
                                        DeploymentId deployment_id) {
  return (Instruction){};
}

bool remus_has_finished_phase(Remus *remus, DeploymentId deployment_id) {
  return true;
}

void remus_finish_deployment_phase(Remus *remus, DeploymentId deployment_id) {}

void remus_finish_reaction_phase(Remus *remus, DeploymentId deployment_id) {}

void remus_finish_phase(Remus *remus, DeploymentId deployment_id) {}

void remus_react(Remus *remus, DeploymentId deployment_id) {}
