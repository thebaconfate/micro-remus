#ifndef REMUS_REACTORS_H
#define REMUS_REACTORS_H

#include "hash_table.h"
#include "reactor.h"

typedef Entry ReactorEntry;
typedef HashTable Reactors;

Reactors reactors_new();

void reactors_free(Reactors *reactors, void (*free_reactor)(void *));

Reactor *reactors_get(const Reactors reactors, Name reactor_name);

void reactors_put(Reactors *reactors, Name reactor_name, Reactor reactor);

#endif
