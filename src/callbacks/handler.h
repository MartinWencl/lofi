#ifndef CALLBACKS_HANDLER_H
#define CALLBACKS_HANDLER_H

#include "callbacks/events.h"
#include "mode_manager.h"
#include "state.h"
 
void HandleEvent(State* state, ModeManager* ModeManager, Event event);

#endif // CALLBACKS_HANDLER_H
