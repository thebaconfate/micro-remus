
#ifndef OPTION_H
#define OPTION_H

struct Value;

typedef enum { NONE, SOME } OptionTag;

typedef struct {
  OptionTag option_tag;
  struct Value *value;
} Option;

#endif
