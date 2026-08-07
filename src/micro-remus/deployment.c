
#include "deployment.h"
#include "abstractions.h"
#include "hash_table.h"
#include "reactor.h"
#include "types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void deployment_write_r(Deployment *deployment, Value value, Number pc);
static void deployment_write_d(Deployment *deployment, Value value, Number n);

Deployments deployments_new() { return NULL; }

void deployments_free(Deployments *deployments) {
  ht_free(deployments, free); // FIX: free isn't the proper freeer, you should
                              // make a proper deployment_free function to free
                              // all the associated fields
}

Deployment *deployments_get(const Deployments deployments,
                            DeploymentId deployment_id) {
  return (Deployment *)ht_get(deployments, &deployment_id,
                              sizeof(DeploymentId));
}

void deployments_put(Deployments *deployments, DeploymentId deployment_id,
                     Deployment deployment) {
  Deployment *new_deployment = malloc(sizeof(Deployment));
  *new_deployment = deployment;
  ht_put(deployments, &deployment, sizeof(DeploymentId), (void *)new_deployment,
         free); // FIX: free probably isn't the prober free in this case, try to
                // figure it out
}

Deployment deployment_new(Name reactor_name, struct Reactor reactor,
                          DeploymentId deployment_id, Scope scope);

Deployment deployment_new(Name reactor_name, Reactor reactor,
                          DeploymentId deployment_id, Scope scope) {

  size_t input_size = reactor_input_size(&reactor);
  size_t output_size = reactor_output_size(&reactor);
  size_t deployment_size = reactor_deployment_size(&reactor);
  size_t reactor_size = reactor_reactor_size(&reactor);

  Inputs inputs = value_option_array_new(input_size);
  Outputs outputs = value_option_array_new(output_size);
  Memory deployment_memory = value_option_array_new(deployment_size);
  Memory reaction_memory = value_option_array_new(reactor_size);

  return (Deployment){.deployment_id = deployment_id,
                      .is_deployed = false,
                      .inputs = inputs,
                      .outputs = outputs,
                      .reactor_name = reactor_name,
                      .deployment_memory = deployment_memory,
                      .reaction_memory = reaction_memory,
                      .pc = 0,
                      .return_address = NULL,
                      .dirty_bit = true,
                      .scope = scope};
}

ValueOption deployment_get_input(Deployment *deployment, Number n) {
  return deployment->inputs.storage[n - 1];
}

Inputs *deployment_get_inputs(Deployment *deployment) {
  return &deployment->inputs;
}

static void deployment_write_input(Deployment *deployment, Number n,
                                   ValueOption input) {
  ValueOption current = deployment->inputs.storage[n];
  if (value_option_equals(current, input)) {
    free(input);
  } else {
    free(current);
    deployment->inputs.storage[n] = input;
    deployment->dirty_bit = true;
  }
}

void deployment_set_input(Deployment *deployment, Number n, Value value) {
  ValueOption value_option = malloc(sizeof(Value));
  *value_option = value;
  deployment_write_input(deployment, n - 1, value_option);
}

ValueOption deployment_get_output(Deployment *deployment, Number n) {
  return deployment->outputs.storage[n - 1];
}

Outputs deployment_get_outputs(Deployment *deployment) {
  return deployment->outputs;
}

void deployment_set_output(Deployment *deployment, Number n, Value value) {
  free(deployment->outputs.storage[n - 1]);
  ValueOption value_option = malloc(sizeof(Value));
  *value_option = value;
  deployment->outputs.storage[n - 1] = value_option;
}

void deployment_set_deployed(Deployment *deployment) {
  deployment->is_deployed = true;
}

void deployment_initialize_pc(Deployment *deployment) { deployment->pc = 0; }

void deployment_increment_pc(Deployment *deployment) { deployment->pc++; }

void deployment_set_return_address(Deployment *deployment,
                                   DeploymentId deployment_id) {
  free(deployment->return_address);
  DeploymentIdOption return_address = malloc(sizeof(DeploymentId));
  if (return_address == NULL) {
    exit(EXIT_FAILURE);
  }
  *return_address = deployment_id;
  deployment->return_address = return_address;
}

void deployment_set_dirty_bit(Deployment *deployment, bool dirty) {
  deployment->dirty_bit = dirty;
}

Scope deployment_get_scope(Deployment *deployment) { return deployment->scope; }

DeploymentId deployment_get_deployment_in_scope(Deployment *deployment,
                                                Number depth) {
  return deployment->scope.storage[depth - 1];
}

void deployment_write(Deployment *deployment, Value value) {
  if (deployment->is_deployed)
    deployment_write_r(deployment, value, deployment->pc);
  else
    deployment_write_d(deployment, value, deployment->pc);
}

void deployment_write_at(Deployment *deployment, Number index, Value value) {
  deployment_write_d(deployment, value, index - 1);
}

void deployment_update_trampoline(Deployment *deployment, Number location,
                                  Value value) {
  deployment_write_d(deployment, value, location - 1);
}

ValueOption deployment_read_d(Deployment *deployment, Number n) {
  return deployment->deployment_memory.storage[n - 1];
}

static void deployment_write_d(Deployment *deployment, Value value, Number n) {
  free(deployment->deployment_memory.storage[n - 1]);
  ValueOption new_value = malloc(sizeof(Value));
  if (new_value == NULL) {
    exit(EXIT_FAILURE);
  }
  deployment->deployment_memory.storage[n - 1] = new_value;
}

ValueOption deployment_read_r(Deployment *deployment, Number n) {
  return deployment->reaction_memory.storage[n - 1];
}

static void deployment_write_r(Deployment *deployment, Value value, Number n) {
  free(deployment->reaction_memory.storage[n - 1]);
  ValueOption new_value = malloc(sizeof(Value));
  if (new_value == NULL) {
    exit(EXIT_FAILURE);
  }
  *new_value = value;
  deployment->reaction_memory.storage[n - 1] = new_value;
}

bool deployment_has_finished_phase(Deployment *deployment) {
  if (deployment->is_deployed)
    return deployment->pc == deployment->reaction_memory.len;
  else
    return deployment->pc == deployment->deployment_memory.len;
}
