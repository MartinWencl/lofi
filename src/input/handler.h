#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "raylib.h"
#include "state.h"
#include "callbacks/events.h"

typedef bool (*InputConditionFunc)(void);
typedef void (*InputStateModifierFunc)(State* state);

#endif // INPUT_HANDLER_H
