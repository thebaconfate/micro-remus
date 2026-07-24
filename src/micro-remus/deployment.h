#include "abstractions.h"
#include "option.h"
#include "types.h"
#include <stdbool.h>

typedef struct DeploymentIdOption {
  OptionTag option_tag;
  DeploymentId deployment_id;
} DeploymentIdOption;

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
