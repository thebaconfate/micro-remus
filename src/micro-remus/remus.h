
#ifndef REMUS_H
#define REMUS_H

#include "types.h"
#include "value.h"
#include <stdbool.h>

typedef struct Remus Remus;

bool remus_read_d(DeploymentId deployment_id, Number n, Value *out_val);
bool remus_read_r(DeploymentId deployment_id, Number n, Value *out_val);
bool remus_get_input(DeploymentId deployment_id, Number n, Value *out_val);
bool remus_get_output(DeploymentId deployment_id, Number n, Value *out_val);

#endif
