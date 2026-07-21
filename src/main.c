
#include "micro-remus/branch.h"
#include "micro-remus/value.h"
#include <stdio.h>
int main() {

  printf("executing main\n");
  Branch *branch = branch_new();
  printf("branch created\n");

  Value val = {.type = NUMBER, .as = {.number = 5}};
  branch_store(branch, "main", val);

  printf("branch stored\n");

  Value *val1 = branch_find(branch, "main");

  if (val1 == NULL) {
    printf("value not found\n");
  }

  Number cint = val1->as.number;

  printf("value: %d\n", (int)cint);

  return 0;
}
