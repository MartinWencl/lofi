// state.c
#include "state.h"
#include "font.h"
#include "raylib.h"
#include <string.h>
#include <stdlib.h>
#include "utils.h"

void InitStateStruct(State* state) {
    memset(state->input, 0, sizeof(state->input));
    state->list = NULL;
    state->listCount = 0;
    state->listScrollIndex = 0;
    state->focus.index = -1;
    
    InitEventQueue(&state->eventQueue);
    InitModeManager(&state->modes);
    
    // Initialize UI state with defaults
    // state->ui.loaded.font = GetFontDefault();
    state->ui.loaded.theme = DEFAULT_THEME;
    state->ui.loaded.window = (Dimensions){0, 0};
    
    state->ui.config.font.name = "monospace";
    state->ui.config.font.size = 12;
    state->ui.config.window = DEFAULT_WINDOW_CONFIG;
}

// TODO: Rework into a cleanup function outside of state, 
// keep only state here, move rest outside
void CleanupAppState(lua_State* L, State* state) {
    TraceLog(LOG_DEBUG, "Starting CleanupAppState");
    
    if (!L) {
        TraceLog(LOG_WARNING, "CleanupAppState: Lua state is NULL");
        return;
    }
    if (!state) {
        TraceLog(LOG_WARNING, "CleanupAppState: State is NULL");
        return;
    }

    CleanupModeManager(L, &state->modes);
    
    TraceLog(LOG_DEBUG, "Cleaning up list view");
    ClearListViewExList(&state->list, &state->listCount);

    TraceLog(LOG_DEBUG, "Unloading font");
    FreeFont(&state->ui);
    
    TraceLog(LOG_DEBUG, "Closing Lua state");
    lua_close(L);
}
