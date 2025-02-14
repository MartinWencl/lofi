#include "callbacks/handler.h"
#include "lua.h"
#include "lua_core.h"
#include "mode_manager.h"
#include "utils.h"

void HandleEvent(lua_State* L, State* state, ModeManager* modeManager, Event event) {
    Mode* mode = modeManager->currentMode;

    if (!modeManager->currentMode) {
        mode = GetCurrentMode(state->input, modeManager);
    }

    if (!mode) {
        TraceLog(LOG_DEBUG, "EVENT: No mode selected or no prefix found, exiting event handling.");
        return;
    }

    DispatchLuaEvent(L, state, mode, event.type);

    // if we need to also do more than just call lua event, 
    // do it here
    switch (event.type) {
        case EVENT_SEARCH_TRIGGERED:
            // Reset focus if results found
            if (state->listCount > 0) {
                state->focus.index = 0;
            }
            break;
        
        case EVENT_FOCUS_CHANGED:
            state->focus.index = event.data.focus.index;
            TraceLog(LOG_DEBUG, "Focus changed, new: %d.", state->focus.index);
            break;
        
        default:
            break;
    }
}
