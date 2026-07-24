#include "value.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void print_value(const Value *v) {
  if (!v)
    return;
  switch (v->type) {
  case VAL_NUMBER:
    printf("%d", (int)v->as.number);
    break;
  case VAL_BOOLEAN:
    printf("%s", v->as.boolean ? "true" : "false");
    break;
  case VAL_REACTOR:
    printf("%s", v->as.reactor ? v->as.reactor : "null");
    break;
  case VAL_BRANCH:
    printf("<Branch %p>", (void *)v->as.branch);
    break;
  }
}

bool value_equals(Value a, Value b) {
  if (a.type != b.type)
    return false;
  switch (a.type) {
  case VAL_NUMBER:
    return a.as.number == b.as.number;
  case VAL_BOOLEAN:
    return a.as.boolean == b.as.boolean;
  case VAL_REACTOR:
    return a.as.reactor == b.as.reactor;
  case VAL_BRANCH:
    return a.as.branch == b.as.branch;
  default:
    return false;
  }
}

bool value_same_type(Value a, Value b) { return a.type == b.type; }

bool value_number(Value a) { return a.type == VAL_NUMBER; }

bool value_numbers(Value a, Value b) {
  return value_number(a) && value_number(b);
}

Value value_add(Value a, Value b) {
  if (value_numbers(a, b))
    return (Value){.type = VAL_NUMBER, .as.number = a.as.number + b.as.number};
  fprintf(stderr, "Error: Incompatible types to perform addition\n");
  exit(EXIT_FAILURE);
}

Value value_mul(Value a, Value b) {
  if (value_numbers(a, b))
    return (Value){.type = VAL_NUMBER, .as.number = a.as.number * b.as.number};
  fprintf(stderr, "Error: Incompatible types to perform multiplication\n");
  exit(EXIT_FAILURE);
}

Value value_sum(const Value *values, size_t count) {
  Value acc = {.type = VAL_NUMBER, .as.number = 0};
  for (size_t i = 0; i < count; i++) {
    if (values[i].type != VAL_NUMBER) {
      fprintf(stderr, "Error: Incompatible types to perform sum\n");
      exit(EXIT_FAILURE);
    }
    acc = (Value){.type = VAL_NUMBER,
                  .as.number = acc.as.number + values[i].as.number};
  }
  return acc;
}

Value value_product(const Value *values, size_t count) {
  Value acc = {.type = VAL_NUMBER, .as.number = 1};
  for (size_t i = 0; i < count; i++) {
    if (values[i].type != VAL_NUMBER) {
      fprintf(stderr, "Error: Incompatible types to perform product\n");
      exit(EXIT_FAILURE);
    }
    acc = (Value){.type = VAL_NUMBER,
                  .as.number = acc.as.number * values[i].as.number};
  }
  return acc;
}
