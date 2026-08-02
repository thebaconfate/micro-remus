#include "signal.h"
#include "hash_table.h"

SignalEnvironment signal_new() { return NULL; }

void signal_free(SignalEnvironment *signal_environment) {
  ht_free(signal_environment, free);
}

ValueOption signal_get(const SignalEnvironment signal_environment,
                       Name signal_name) {
  return (ValueOption)ht_get(signal_environment, signal_name,
                             strlen(signal_name));
}

void signal_put(SignalEnvironment *signal_environment, Name signal_name,
                Value value) {
  ValueOption new_val = malloc(sizeof(Value));
  *new_val = value;
  ht_put(signal_environment, signal_name, strlen(signal_name), (void *)new_val,
         free);
}
