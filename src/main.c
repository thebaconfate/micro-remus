
#include "micro-remus/branch.h"
#include "micro-remus/types.h"
#include "micro-remus/value.h"
#include <stdio.h>
int main() {

  printf("executing main\n");
  printf("branch created\n");

  Value val = {.type = VAL_NUMBER, .as = {.number = 5}};
  Name name = "Main";

  branch_add(name, val);

  printf("branch stored\n");

  Value *v = branch_find(name);

  if (v == NULL) {
    printf("Branch not found\n");
  }

  printf("value: %d\n", (int)v->as.number);

  return 0;
}
