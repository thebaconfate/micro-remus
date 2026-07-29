
#include "src/micro-remus/branch.h"
#include "src/micro-remus/types.h"
#include "src/micro-remus/value.h"
#include <stdio.h>
int main() {

  printf("executing main\n");
  printf("branch created\n");

  Value val = {.type = VAL_NUMBER, .as = {.number = 5}};
  Name name = "Main";
  Branch branch = branch_new();

  branch_add(&branch, name, val);

  printf("branch stored\n");

  ValueOption v = branch_find(branch, name);

  if (v.option_tag == NONE) {
    printf("Branch not found\n");
  }

  printf("value: %d\n", (int)v.value->as.number);

  return 0;
}
