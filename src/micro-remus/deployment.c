
#include "deployment.h"
#include "abstractions.h"
#include "reactor.h"
#include "types.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Deployment deployment_new(Name reactor_name, Reactor reactor,
                          DeploymentId deployment_id, Scope scope) {
  Inputs inputs = value_option_array_new(reactor_input_size(&reactor));
  Outputs outputs = value_option_array_new(reactor_output_size(&reactor));
  Memory deployment_memory =
      value_option_array_new(reactor_deployment_size(&reactor));
  Memory reaction_memory =
      value_option_array_new(reactor_reactor_size(&reactor));
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
  size_t idx = n - 1;
  if (idx < 0) {
    fprintf(stderr, "Cannot get input of negative index\n");
    exit(EXIT_FAILURE);
  }
  if (deployment->inputs.len < idx) {
    fprintf(stderr, "Cannot get %dth input if input size is %d", (int)idx,
            (int)deployment->inputs.len);
    exit(EXIT_FAILURE);
  }
  return deployment->inputs.storage[idx];
}

Inputs *deployment_get_inputs(Deployment *deployment) {
  return &deployment->inputs;
}

void deployment_write_input(Deployment *deployment, Number n,
                            ValueOption input) {
  ValueOption current = deployment->inputs.storage[n];
  // TODO: Continue here
  if (!value_option_equals(current, input)) {
    deployment->inputs.storage[n] = input;
    deployment->dirty_bit = true;
  }
}

void deployment_set_input(Deployment *deployment, Number n, Value value) {
  deployment_write_input(deployment, n - 1,
                         (ValueOption){.option_tag = SOME, &value});
}

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
