#ifndef REMUS_SIGNAL_H
#define REMUS_SIGNAL_H

#include "hash_table.h"
#include "value.h"

typedef Entry SignalEntry;
typedef HashTable SignalEnvironment;

SignalEnvironment signal_new();

void signal_free(SignalEnvironment *signal_environment);

ValueOption signal_get(const SignalEnvironment signal_environment,
                       Name signal_name);

void signal_put(SignalEnvironment *signal_environment, Name signal_name,
                Value value);

#endif
