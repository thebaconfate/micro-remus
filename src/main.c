
#include "micro-remus/branch.h"
#include "micro-remus/types.h"
#include "micro-remus/value.h"
#include <stdio.h>
int main() {

  printf("executing main\n");
  printf("branch created\n");

  Value val = {.type = NUMBER, .as = {.number = 5}};
  Name name = "Main";

  add_branch(name, val);

  printf("branch stored\n");

  Branch *b = find_branch(name);

  if (b == NULL) {
    printf("Branch not found\n");
  }

  Value val1 = b->val;

  printf("value: %d\n", (int)val1.as.number);

  return 0;
}
