#ifndef REMUS_LOCATION_H
#define REMUS_LOCATION_H

#include "types.h"
#include "value.h"
#include <stdbool.h>

struct Remus;

typedef enum {
  LOC_D,
  LOC_R,
  LOC_I,
  LOC_O,
} LocationType;

typedef struct Location {
  LocationType type;
  Number index;
} Location;

/**
 * @brief Fetches a Value from Remus memory based on location type and index.
 *
 * @param loc The location descriptor (D, R, I, or O memory).
 * @param deployment_id The ID of the deployment to query.
 * @param remus Pointer to the global/context Remus engine instance.
 * @param out_val Pointer to a Value struct where the result will be written.
 *
 * @return true if a value was found and written to out_val, false otherwise
 * (Option::None).
 */
ValueOption location_fetch(const Location *loc, DeploymentId deployment_id,
                           struct Remus *remus);

#endif
