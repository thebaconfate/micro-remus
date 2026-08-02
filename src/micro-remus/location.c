#include "location.h"
#include "remus.h"

Value *location_fetch(const Location *loc, DeploymentId deployment_id,
                      Remus *remus) {
  if (!loc || !remus)
    return NULL;
  switch (loc->type) {
  case LOC_D:
    return remus_read_d(remus, deployment_id, loc->index);
  case LOC_R:
    return remus_read_r(remus, deployment_id, loc->index);
  case LOC_I:
    return remus_get_input(remus, deployment_id, loc->index);
  case LOC_O:
    return remus_get_output(remus, deployment_id, loc->index);
  default:
    return (ValueOption){.option_tag = NONE, .value = NULL};
  }
}
