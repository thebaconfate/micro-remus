#include "location.h"
#include "option.h"
#include "remus.h"
#include "value.h"

ValueOption location_fetch(const Location *loc, DeploymentId deployment_id,
                           Remus *remus, Value *out_val) {
  if (!loc || !remus || !out_val)
    return (ValueOption){.option_tag = NONE};
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
    return (ValueOption){.option_tag = NONE};
  }
}
