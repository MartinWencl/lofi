#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "raylib.h"
#include "state.h"
#include "callbacks/events.h"

typedef bool (*InputConditionFunc)(void);
typedef Event (*InputEventGeneratorFunc)(State* state);
typedef void (*InputStateModifierFunc)(State* state);

void Temp();

#endif // INPUT_HANDLER_H
