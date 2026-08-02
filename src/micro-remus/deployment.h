#ifndef REMUS_DEPLOYMENT_H
#define REMUS_DEPLOYMENT_H

#include "abstractions.h"
#include "types.h"
#include "value.h"
#include <stdbool.h>

struct Reactor;

typedef DeploymentId *DeploymentIdOption;

typedef struct Deployment {
  DeploymentId deployment_id;
  bool is_deployed;
  Inputs inputs;
  Outputs outputs;
  Name reactor_name;
  Memory deployment_memory;
  Memory reaction_memory;
  size_t pc;
  DeploymentIdOption return_address;
  bool dirty_bit;
  Scope scope;
} Deployment;

Deployment deployment_new(Name reactor_name, struct Reactor reactor,
                          DeploymentId deployment_id, Scope scope);

ValueOption deployment_get_input(Deployment *deployment, Number n);

Inputs *deployment_get_inputs(Deployment *deployment);

void deployment_write_input(Deployment *deployment, Number n,
                            ValueOption input);

void deployment_set_input(Deployment *deployment, Number n, Value value);

ValueOption deployment_get_output(Deployment *deployment, Number n);

Outputs *deployment_get_outputs(Deployment *deployment);

void deployment_set_output(Deployment *deployment, Number n, Value value);

void deployment_set_deployed(Deployment *deployment);

void deployment_initialize_pc(Deployment *deployment);

void deployment_increment_pc(Deployment *deployment);

void deployment_set_return_address(Deployment *deployment,
                                   DeploymentId deployment_id);

void deployment_set_dirty_bit(Deployment *deployment, bool dirty);

Scope *deployment_get_scope(Deployment *deployment);

DeploymentId deployment_get_deployment_in_scope(Deployment *deployment,
                                                Number depth);

void deployment_write(Deployment *deployment, Value value);

void deployment_write_at(Deployment *deployment, Number index, Value value);

void deployment_update_trampoline(Deployment *deployment, Number location,
                                  Value value);

ValueOption deployment_read_d(Deployment *deployment, Number n);

void deployment_write_d(Deployment *deployment, Value value, Number n);

ValueOption deployment_read_r(Deployment *deployment, Number n);

void deployment_write_r(Deployment *deployment, Value value, Number n);

bool deployment_has_finished_phase(Deployment *deployment);

#endif
