#include "reactors.h"
#include "hash_table.h"

Reactors reactors_new() { return NULL; }

void reactors_free(Reactors *reactors, void (*free_reactor)(void *)) {
  ht_free(reactors, free_reactor);
}

Reactor *reactors_get(const Reactors reactors, Name reactor_name) {
  return (Reactor *)ht_get(reactors, reactor_name, strlen(reactor_name));
}

void reactors_put(Reactors *reactors, Name reactor_name, Reactor reactor) {
  Reactor *new_reactor = malloc(sizeof(Reactor));
  *new_reactor = reactor;
  ht_put(reactors, reactor_name, strlen(reactor_name), (void *)new_reactor,
         free);
}
