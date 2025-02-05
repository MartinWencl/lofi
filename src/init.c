#include "state.h"
#include "lua_core.h"
// #include "lua/window.h"
// #include "lua/theme.h"
#include "ui/window.h"
// #include "lua/font.h"
#include "lua.h"
#include <stdbool.h>

void PrintConfiguration(const State* state) {
    if (state == NULL) {
        TraceLog(LOG_WARNING, "Invalid state or configuration.");
        return;
    }

    TraceLog(LOG_INFO, "Window Configuration:");
    TraceLog(LOG_INFO, "Refresh Rate: %d", state->ui.config.window.refreshRate);
    TraceLog(LOG_INFO, "Window Width Percent: %.2f", state->ui.config.window.windowPercentWidth);
    TraceLog(LOG_INFO, "Window Height Percent: %.2f", state->ui.config.window.windowPercentHeight);
    TraceLog(LOG_INFO, "Opacity: %.2f", state->ui.config.window.opacity);
    TraceLog(LOG_INFO, "Decoration: %s", state->ui.config.window.decoration ? "true" : "false");

    TraceLog(LOG_INFO, "Font Configuration:");
    TraceLog(LOG_INFO, "Name: %s", state->ui.config.font.name);
    TraceLog(LOG_INFO, "Size: %d", state->ui.config.font.size);
}

bool TryInitialize(State* state, lua_State* L) {
    InitStateStruct(state);

    InitLua("init.lua", &state->modes, L); 
    if(!L) {
        TraceLog(LOG_ERROR, "Failed to initialize lua!");
    }

    LoadOptionsFromLua(state, L);

    TraceLog(LOG_ERROR, "TEST OPACITY: %f", state->ui.config.window.opacity);
     
    Dimensions window = InitWindowFromConfig(&state->ui.config.window, "lofi");
    state->ui.loaded.window = window;

    LoadFontFromState(&state->ui);
    PrintConfiguration(state);
}
