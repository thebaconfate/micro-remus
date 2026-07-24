
#ifndef REMUS_H
#define REMUS_H

#include "abstractions.h"
#include "deployment.h"
#include "location.h"
#include "option.h"
#include "program.h"
#include "reactor.h"
#include "types.h"
#include "value.h"
#include <stdbool.h>

typedef struct Remus {
  // Hardware hardware
  DeploymentId main_deployment_id;
  Reactors reactors;
  Deployments deployments;
  SignalEnvironment global_signals;
} Remus;

Remus remus_new(Program program);
void remus_start(Remus *remus);
Number remus_increment_time(Remus *remus);
void remus_actuate(Remus *remus, Outputs outputs); // Check signature of outputs

/*----------- reactors -------------*/
Reactor *remus_get_reactor(Remus *remus, Name reactor_name);
void remus_deploy(Remus *remus, Name reactor_name, DeploymentId deployment_id,
                  Scope Scope);

/*----------- signals -------------*/

ValueOption remus_get_signal(Remus *remus, Name signal_name);

/*----------- deployments -------------*/

Deployment remus_get_deployment(Remus *remus, DeploymentId deployment_id);
void remus_insert_deployment(Remus *remus, DeploymentId deployment_id,
                             Deployment deployment);
bool remus_is_deployed(Remus *remus, DeploymentId deployment_id);
void remus_set_deployed(Remus *remus, DeploymentId deployment_id);
void remus_initialize_pc(Remus *remus, DeploymentId deployment_id);
void remus_increment_pc(Remus *remus, DeploymentId deployment_id);
DeploymentIdOption remus_get_return_address(Remus *remus,
                                            DeploymentId deployment_id);
void remus_set_return_address(Remus *remus, DeploymentId deployment_id,
                              DeploymentId return_address);
bool remus_get_dirty_bit(Remus *remus, DeploymentId deployment_id);

void remus_set_dirty_bit(Remus *remus, DeploymentId deployment_id, bool dirty);

bool remus_is_dirty(Remus *remus, DeploymentId deployment_id);

Scope remus_get_scope(Remus *remus, DeploymentId deployment_id);

DeploymentId remus_get_deployment_in_scope(Remus *remus,
                                           DeploymentId deployment_id,
                                           Number depth);

ValueOption remus_get_input(Remus *remus, DeploymentId deployment_id, Number n);
Inputs *remus_get_inputs(Remus *remus, DeploymentId deployment_id);

void remus_set_input(Remus *remus, DeploymentId deployment_id, Number n,
                     Value value);

ValueOption remus_get_output(Remus *remus, DeploymentId deployment_id,
                             Number n);

Outputs remus_get_outputs(Remus *remus, DeploymentId deployment_id);

void remus_set_output(Remus *remus, DeploymentId deployment_id, Number n,
                      Value value);

Command remus_get_deployment_command(Remus *remus, DeploymentId deployment_id);

ValueOption remus_read_d(Remus *remus, DeploymentId deployment_id, Number n);

Command remus_get_reaction_command(Remus *remus, DeploymentId deployment_id);

ValueOption remus_read_r(Remus *remus, DeploymentId deployment_id, Number n);

void remus_write(Remus *remus, DeploymentId deployment_id, Value value);

void remus_write_at(Remus *remus, DeploymentId deployment_id, Number index,
                    Value value);

void remus_update_trampoline(Remus *remus, DeploymentId deployment_id,
                             Number location, Value value);

Command remus_retrieve_next_command(Remus *remus, DeploymentId deployment_id);

bool remus_has_finished_phase(Remus *remus, DeploymentId deployment_id);

void remus_finish_deployment_phase(Remus *remus, DeploymentId deployment_id);

void remus_finish_reaction_phase(Remus *remus, DeploymentId deployment_id);

void remus_finish_phase(Remus *remus, DeploymentId deployment_id);

void remus_react(Remus *remus, DeploymentId deployment_id);

#endif
