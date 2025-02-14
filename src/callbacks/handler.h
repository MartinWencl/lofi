#ifndef CALLBACKS_HANDLER_H
#define CALLBACKS_HANDLER_H

#include "callbacks/events.h"
#include "lua.h"
#include "mode_manager.h"
#include "state.h"
 
void HandleEvent(lua_State* L, State* state, ModeManager* ModeManager, Event event);

#endif // CALLBACKS_HANDLER_H
