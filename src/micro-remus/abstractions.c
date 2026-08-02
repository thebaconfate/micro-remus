#include "abstractions.h"
#include "value.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

ValueArray value_option_array_new(size_t size) {
  ValueOption *storage = calloc(size, sizeof(ValueOption));
  if (storage == NULL && size > 0) {
    fprintf(stderr, "Fatal: Out of memory in value_option_array_new\n");
    exit(EXIT_FAILURE);
  }
  return (ValueArray){.storage = storage, .len = size, .capacity = size};
}

void value_option_array_free(ValueArray *array) {
  if (array == NULL)
    return;
  for (size_t i = 0; i < array->len; i++) {
    if (array->storage[i] != NULL)
      free(array->storage[i]);
  }
  free(array->storage);
  array->storage = NULL;
  array->len = 0;
  array->capacity = 0;
}
