#include "state.h"
#include "lua.h"
#include "mode_manager.h"
#include "raylib.h"
#include <stdlib.h>
#include "stddef.h"
#include "assert.h"

[[nodiscard]]
State* NewState(void)
{
    State* state = (State*)malloc(sizeof(State));
    if (state == nullptr) {
        TraceLog(LOG_ERROR, "Failed to create app state!");
        return nullptr;
    }
    
    *state = (State){
        .input = {0},
        .list = nullptr,
        .listCount = 0,
        .listScrollIndex = 0,
        .keybindStore = NewKeybindStore(),
        .focus = {.index = -1},
        .eventQueue = NewEventQueue(),
        .modes = NewModeManager(),
        .ui = {0},
    };
    
    return state;
}

void FreeState(lua_State* L, State* state) {
    assert(state != nullptr);
    assert(L != nullptr);

    if (state == nullptr) {
        TraceLog(LOG_ERROR, "A call to free an alredy freed app state!");
        return;
    }

    FreeModeManager(L, &state->modes);
    free(state);
}
