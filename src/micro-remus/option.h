
#ifndef OPTION_H
#define OPTION_H

#include "value.h"

typedef enum { NONE, SOME } OptionTag;

typedef struct {
  OptionTag option_tag;
  Value *value;
} Option;

#endif
