#ifndef INPUT_DEFINITIONS_H
#define INPUT_DEFINITIONS_H

#include "state.h"
#include "callbacks/events.h"
#include <stdbool.h>

void ProcessInput(State* state, EventQueue* eventQueue, lua_State* L);

#endif // INPUT_DEFINITIONS_H
