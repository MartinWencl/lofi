#ifndef STATE_H
#define STATE_H

#include "callbacks/events.h"
#include "lua.h"
#include "mode_manager.h"
#include "ui/state.h"

#include "input/keybinds.h"

#define INPUT_TEXT_MAX_SIZE 256
#define MAX_NUMBER_OF_ESC_KEYS 64

typedef struct {
    int index; 
} Focus;

typedef struct {
    char input[INPUT_TEXT_MAX_SIZE];

    char **list;
    int listCount;
    int listScrollIndex;

    KeybindStore keybindStore; 

    Focus focus;
    // FocusManager focus;
    EventQueue eventQueue;
    ModeManager modes;

    UIState ui;
} State;

State* NewState(void);
void FreeState(State*);

#endif // STATE_H
