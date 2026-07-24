#include "location.h"
#include "abstractions.h"
#include "remus.h"
#include "value.h"

bool location_fetch(const Location *loc, DeploymentId deployment_id,
                    const Remus *remus, Value *out_val) {
  if (!loc || !remus || !out_val)
    return false;
  switch (loc->type) {
  case LOC_D:
    return remus_read_d(deployment_id, loc->index, out_val);
  case LOC_R:
    return remus_read_r(deployment_id, loc->index, out_val);
  case LOC_I:
    return remus_get_input(deployment_id, loc->index, out_val);
  case LOC_O:
    return remus_get_output(deployment_id, loc->index, out_val);
  default:
    return false;
  }
}
