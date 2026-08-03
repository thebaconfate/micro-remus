#include "operand.h"
#include "location.h"
#include <stdio.h>
#include <stdlib.h>

ValueOption operand_fetch(Operand *operand, DeploymentId deployment_id,
                          struct Remus *remus) {
  switch (operand->tag) {
  case OPERAND_VALUE:
    return &operand->as.value;
  case OPERAND_LOCATION:
    return location_fetch(&operand->as.location, deployment_id, remus);
  default:
    fprintf(stderr, "Error: unknown operand type\n");
    exit(EXIT_FAILURE);
  }
}
