
#include "src/micro-remus/branch.h"
#include "src/micro-remus/types.h"
#include "src/micro-remus/value.h"
#include <stdio.h>
int main() {

  printf("executing main\n");
  printf("branch created\n");

  Value val = {.type = VAL_NUMBER, .as = {.number = 5}};
  printf("Storing the number with value %d\n", (int)val.as.number);

  Name name = "Main";
  Branch branch = branch_new();

  branch_put(&branch, name, val);

  printf("branch stored\n");

  ValueOption v = branch_get(branch, name);

  if (v == NULL) {
    printf("Branch not found\n");
  }

  printf("value: %d\n", (int)v->as.number);

  return 0;
}
