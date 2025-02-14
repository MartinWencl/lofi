#ifndef INPUT_DEFINITIONS_H
#define INPUT_DEFINITIONS_H

#include "state.h"
#include "callbacks/events.h"
#include <stdbool.h>

#define MAX_ACTIVE_MODIFIERS 3

typedef enum {
    MOD_ALT,
    MOD_CTRL,
    MOD_SUPER,
} Modifier;

typedef struct Key {
    int key;
    Modifier active_modifiers[MAX_ACTIVE_MODIFIERS];
} Key;

bool CheckKey(Key key);

void ProcessInput(State* state, EventQueue* eventQueue);

#endif // INPUT_DEFINITIONS_H
