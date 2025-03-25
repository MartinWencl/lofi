#ifndef STATE_H
#define STATE_H

#include "state_declaration.h"
#include "callbacks/events.h"
#include "mode_manager.h"
#include "ui/state.h"
#include "input/keybinds.h"

#define INPUT_TEXT_MAX_SIZE 256
#define MAX_NUMBER_OF_ESC_KEYS 64

typedef struct {
    int index; 
} Focus;

struct State {
    char input[INPUT_TEXT_MAX_SIZE];
    char **list;
    int listCount;
    int listScrollIndex;
    KeybindStore keybindStore; 
    Focus focus;
    EventQueue eventQueue;
    ModeManager modes;
    UIState ui;
};

State* NewState(void);
void FreeState(State*);

#endif // STATE_H
