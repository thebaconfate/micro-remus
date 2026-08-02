#include "abstractions.h"
#include "option.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ValueOptionArray value_option_array_new(size_t size) {
  ValueOption *storage = malloc(sizeof(ValueOption) * size);
  if (storage == NULL && size > 0) {
    fprintf(stderr, "Fatal: Out of memory in value_option_array_new\n");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < size; i++) {
    storage[i] = (ValueOption){.option_tag = NONE, .value = NULL};
  }
  return (ValueOptionArray){.storage = storage, .len = size, .capacity = size};
}

void value_option_array_free(ValueOptionArray *array) {
  if (array == NULL)
    return;
  free(array->storage);
  array->storage = NULL;
  array->len = 0;
  array->capacity = 0;
}
