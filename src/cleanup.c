#include "cleanup.h"
#include "mode_manager.h"
#include "utils.h"

#include "raylib.h"

void Cleanup(lua_State *L, State *state)
{
    TraceLog(LOG_DEBUG, "Starting final cleanup.");
    if (!L) {
        TraceLog(LOG_ERROR, "CleanupAppState: Lua state is NULL");
        return;
    }
    if (!state) {
        TraceLog(LOG_ERROR, "CleanupAppState: State is NULL");
        return;
    }

    ClearListViewExList(&state->list, &state->listCount);

    CleanupAppState(L, state);
    
    lua_close(L);
    CloseWindow();
}