#ifndef STATE_H
#define STATE_H

#include "callbacks/events.h"
#include "modes.h"
// #include "ui/focus.h"
#include "ui/state.h"

#define INPUT_TEXT_MAX_SIZE 256

typedef struct {
    int index; 
} Focus;

typedef struct {
    char input[INPUT_TEXT_MAX_SIZE];
    char **list;
    int listCount;

    Focus focus;
    // FocusManager focus;
    EventQueue eventQueue;
    ModeManager modes;

    UIState ui;
} State;

void InitStateStruct(State* state);
void CleanupAppState(State* state);

#endif // STATE_H
