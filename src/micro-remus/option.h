
#ifndef OPTION_H
#define OPTION_H

typedef struct Value Value;

typedef enum { NONE, SOME } OptionTag;

typedef struct {
  OptionTag option_tag;
  Value *value;
} Option;

#endif
